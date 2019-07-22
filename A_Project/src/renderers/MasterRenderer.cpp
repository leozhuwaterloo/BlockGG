#include "./includes/MasterRenderer.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include "cs488-framework/GlErrorCheck.hpp"
#include "../managers/includes/GameManager.hpp"

MasterRenderer::MasterRenderer(MasterShader *shader)
	: Renderer(shader),
		m_shader(shader) {}
MasterRenderer::~MasterRenderer() {}

void MasterRenderer::updateGlobalShaderUniforms() {
	glUniformMatrix4fv(m_shader->view_loc, 1, GL_FALSE, glm::value_ptr(resource_manager->m_view));
	glUniform3fv(m_shader->view_position_loc, 1, glm::value_ptr(resource_manager->game->player->pos));
	//-- Set LightSource uniform for the scene:
	glUniform3fv(m_shader->light_position_loc, 1, glm::value_ptr(resource_manager->m_light->pos));
  glUniformMatrix4fv(m_shader->l_vp_matrix_loc, 1, GL_FALSE, glm::value_ptr(resource_manager->l_vp_matrix));
	CHECK_GL_ERRORS;
}


void MasterRenderer::updateShaderUniforms(const GeometryNode &node) {
	const std::string &textureId = node.material.textureId;
	if (textureId.empty() || !resource_manager->game->command_manager->render_texture) {
		glActiveTexture(GL_TEXTURE0 + m_shader->m_texture_index);
		glBindTexture(GL_TEXTURE_2D, 0);
		glUniform1i(m_shader->using_texture_loc, 0);
	} else {
		glActiveTexture(GL_TEXTURE0 + m_shader->m_texture_index);
		glBindTexture(GL_TEXTURE_2D, resource_manager->texture_map.at(textureId));
		glUniform1i(m_shader->using_texture_loc, 1);
	}
  //-- Set model and view matrix:
  glUniformMatrix4fv(m_shader->model_loc, 1, GL_FALSE, glm::value_ptr(node.trans));

  //-- Set Material values:
  glm::vec3 kd = node.material.kd;
  glUniform3fv(m_shader->material_kd_loc, 1, glm::value_ptr(kd));
  glm::vec3 ks = node.material.ks;
  glUniform3fv(m_shader->material_ks_loc, 1, glm::value_ptr(ks));
  glUniform1f(m_shader->material_shininess_loc, node.material.shininess);
	CHECK_GL_ERRORS;
}


void MasterRenderer::onEnable() {
  glBindBuffer(GL_ARRAY_BUFFER, resource_manager->m_mesh->vbo_position);
  glVertexAttribPointer(m_shader->position_attr_loc, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
  glEnableVertexAttribArray(m_shader->position_attr_loc);

  glBindBuffer(GL_ARRAY_BUFFER, resource_manager->m_mesh->vbo_normal);
  glVertexAttribPointer(m_shader->normal_attr_loc, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
  glEnableVertexAttribArray(m_shader->normal_attr_loc);

  glBindBuffer(GL_ARRAY_BUFFER, resource_manager->m_mesh->vbo_uv_coord);
  glVertexAttribPointer(m_shader->uv_coord_attr_loc, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(m_shader->uv_coord_attr_loc);

  glVertexAttribDivisor(m_shader->pos_offset_attr_loc, 1);
	CHECK_GL_ERRORS;
}

void MasterRenderer::onDisable() {
  glDisableVertexAttribArray(m_shader->position_attr_loc);
  glDisableVertexAttribArray(m_shader->normal_attr_loc);
  glDisableVertexAttribArray(m_shader->uv_coord_attr_loc);
  glDisableVertexAttribArray(m_shader->pos_offset_attr_loc);
	glVertexAttribDivisor(m_shader->pos_offset_attr_loc, 0);
	CHECK_GL_ERRORS;
}
