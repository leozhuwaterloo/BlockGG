#include "./includes/NormalRenderer.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include "cs488-framework/GlErrorCheck.hpp"
#include "../managers/includes/GameManager.hpp"

NormalRenderer::NormalRenderer(NormalShader *shader)
	: Renderer(shader),
		n_shader(shader) {}
NormalRenderer::~NormalRenderer() {}

void NormalRenderer::updateGlobalShaderUniforms() {
	glUniformMatrix4fv(n_shader->view_loc, 1, GL_FALSE, glm::value_ptr(resource_manager->m_view));
	glUniform3fv(n_shader->view_position_loc, 1, glm::value_ptr(resource_manager->game->player->pos));
	//-- Set LightSource uniform for the scene:
	glUniform3fv(n_shader->light_position_loc, 1, glm::value_ptr(resource_manager->m_light->pos));
  glUniformMatrix4fv(n_shader->l_vp_matrix_loc, 1, GL_FALSE, glm::value_ptr(resource_manager->l_vp_matrix));
	CHECK_GL_ERRORS;
}


void NormalRenderer::updateShaderUniforms(const GeometryNode &node) {
	const std::string &textureId = node.material.textureId;
	if (textureId.empty() || !resource_manager->game->command_manager->render_texture) {
		glActiveTexture(GL_TEXTURE0 + n_shader->m_texture_index);
		glBindTexture(GL_TEXTURE_2D, 0);
		glUniform1i(n_shader->using_texture_loc, 0);
	} else {
		glActiveTexture(GL_TEXTURE0 + n_shader->m_texture_index);
		glBindTexture(GL_TEXTURE_2D, resource_manager->texture_map.at(textureId));
		glUniform1i(n_shader->using_texture_loc, 1);
	}
  //-- Set model and view matrix:
  glUniformMatrix4fv(n_shader->model_loc, 1, GL_FALSE, glm::value_ptr(node.trans));

  //-- Set Material values:
  glm::vec3 kd = node.material.kd;
  glUniform3fv(n_shader->material_kd_loc, 1, glm::value_ptr(kd));
  glm::vec3 ks = node.material.ks;
  glUniform3fv(n_shader->material_ks_loc, 1, glm::value_ptr(ks));
  glUniform1f(n_shader->material_shininess_loc, node.material.shininess);
	CHECK_GL_ERRORS;
}


void NormalRenderer::onEnable() {
  glBindBuffer(GL_ARRAY_BUFFER, resource_manager->m_mesh->vbo_position);
  glVertexAttribPointer(n_shader->position_attr_loc, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
  glEnableVertexAttribArray(n_shader->position_attr_loc);

  glBindBuffer(GL_ARRAY_BUFFER, resource_manager->m_mesh->vbo_normal);
  glVertexAttribPointer(n_shader->normal_attr_loc, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
  glEnableVertexAttribArray(n_shader->normal_attr_loc);

  glBindBuffer(GL_ARRAY_BUFFER, resource_manager->m_mesh->vbo_uv_coord);
  glVertexAttribPointer(n_shader->uv_coord_attr_loc, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(n_shader->uv_coord_attr_loc);

	glBindBuffer(GL_ARRAY_BUFFER, resource_manager->m_mesh->vbo_tangent);
  glVertexAttribPointer(n_shader->tangent_attr_loc, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(n_shader->tangent_attr_loc);

  glVertexAttribDivisor(n_shader->pos_offset_attr_loc, 1);
	CHECK_GL_ERRORS;
}

void NormalRenderer::onDisable() {
  glDisableVertexAttribArray(n_shader->position_attr_loc);
  glDisableVertexAttribArray(n_shader->normal_attr_loc);
  glDisableVertexAttribArray(n_shader->uv_coord_attr_loc);
	glDisableVertexAttribArray(n_shader->tangent_attr_loc);
  glDisableVertexAttribArray(n_shader->pos_offset_attr_loc);
	glVertexAttribDivisor(n_shader->pos_offset_attr_loc, 0);
	CHECK_GL_ERRORS;
}
