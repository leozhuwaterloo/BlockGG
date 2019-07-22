#include "./includes/ShadowRenderer.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include "cs488-framework/GlErrorCheck.hpp"
#include "../managers/includes/GameManager.hpp"

ShadowRenderer::ShadowRenderer(ShadowShader *shader)
	: Renderer(shader),
		s_shader(shader),
		shadow_width(3000),
		shadow_height(3000)
{
	glGenFramebuffers(1, &fbo_depth_map);

	glGenTextures(1, &depth_map);
	glBindTexture(GL_TEXTURE_2D, depth_map);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		shadow_width, shadow_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glBindFramebuffer(GL_FRAMEBUFFER, fbo_depth_map);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_map, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	CHECK_GL_ERRORS;
}

ShadowRenderer::~ShadowRenderer() {}

void ShadowRenderer::updateShaderUniforms(const GeometryNode &node) {
  glm::mat4 l_mvp_matrix = resource_manager->l_vp_matrix * node.trans;
  glUniformMatrix4fv(s_shader->l_mvp_matrix_loc, 1, GL_FALSE, glm::value_ptr(l_mvp_matrix));
}


void ShadowRenderer::onEnable() {
  glBindBuffer(GL_ARRAY_BUFFER, resource_manager->m_mesh->vbo_position);
  glVertexAttribPointer(s_shader->position_attr_loc, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
  glEnableVertexAttribArray(s_shader->position_attr_loc);

  glEnableVertexAttribArray(s_shader->pos_offset_attr_loc);
  glVertexAttribDivisor(s_shader->pos_offset_attr_loc, 1);

	glViewport(0, 0, shadow_width, shadow_height);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo_depth_map);
	glClear(GL_DEPTH_BUFFER_BIT);
}

void ShadowRenderer::onDisable() {;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, resource_manager->game->m_windowWidth, resource_manager->game->m_windowHeight);
  glDisableVertexAttribArray(s_shader->position_attr_loc);
  glDisableVertexAttribArray(s_shader->pos_offset_attr_loc);
	glVertexAttribDivisor(s_shader->pos_offset_attr_loc, 0);
}
