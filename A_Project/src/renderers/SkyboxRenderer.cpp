#include "./includes/SkyboxRenderer.hpp"
#include "cs488-framework/GlErrorCheck.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

SkyboxRenderer::SkyboxRenderer(SkyboxShader *shader)
  : Renderer(shader),
    b_shader(shader) {}
SkyboxRenderer::~SkyboxRenderer() {}

void SkyboxRenderer::onEnable() {
  glDisable(GL_CULL_FACE);
  glDepthMask(GL_FALSE);
  glDepthRange(2.0f, 2.0f);
  glBindBuffer(GL_ARRAY_BUFFER, resource_manager->m_mesh->vbo_position);
  glVertexAttribPointer(b_shader->position_attr_loc, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
  glEnableVertexAttribArray(b_shader->position_attr_loc);

  glm::mat4 view = resource_manager->m_view;
  view = glm::scale(view, glm::vec3(100.0f, 100.0f, 100.0f));
  view = view * glm::rotate(glm::radians(-83.0f), glm::vec3(0.0f, 1.0f, 0.0f));
  view[3][0] = 0.0f;
  view[3][1] = 0.0f;
  view[3][2] = 0.0f;
  glUniformMatrix4fv(b_shader->view_loc, 1, GL_FALSE, glm::value_ptr(view));
  glBindTexture(GL_TEXTURE_CUBE_MAP, resource_manager->texture_map.at("skybox"));
	CHECK_GL_ERRORS;
}

void SkyboxRenderer::onDisable() {
  glEnable(GL_CULL_FACE);
  glDepthRange(0.0f, 1.0f);
  glDepthMask(GL_TRUE);
  glDisableVertexAttribArray(b_shader->position_attr_loc);
	CHECK_GL_ERRORS;
}
