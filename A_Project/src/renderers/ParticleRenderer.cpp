#include "./includes/ParticleRenderer.hpp"
#include "cs488-framework/GlErrorCheck.hpp"


ParticleRenderer::ParticleRenderer(ParticleShader *shader)
  : Renderer(shader),
    p_shader(shader) {}
ParticleRenderer::~ParticleRenderer() {}

void ParticleRenderer::onEnable() {
  glBindBuffer(GL_ARRAY_BUFFER, resource_manager->m_mesh_2d->vbo_position);
  glVertexAttribPointer(p_shader->position_attr_loc, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
  glEnableVertexAttribArray(p_shader->position_attr_loc);

  for(int i = 0; i < 4; ++i) {
    glVertexAttribDivisor(p_shader->model_view_attr_loc + i, 1);
  }
  glBlendFunc(GL_SRC_ALPHA, GL_ONE);
  CHECK_GL_ERRORS;
}

void ParticleRenderer::onDisable() {
  glDisableVertexAttribArray(p_shader->position_attr_loc);
  glDisableVertexAttribArray(p_shader->model_view_attr_loc);

  for(int i = 0; i < 4; ++i) {
    glVertexAttribDivisor(p_shader->model_view_attr_loc + i, 0);
  }
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  CHECK_GL_ERRORS;
}
