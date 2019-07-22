#include "./includes/GuiRenderer.hpp"

#include "cs488-framework/GlErrorCheck.hpp"

GuiRenderer::GuiRenderer(GuiShader *shader)
  : Renderer(shader),
    g_shader(shader) {}
GuiRenderer::~GuiRenderer() {}

void GuiRenderer::onEnable() {
  glBindBuffer(GL_ARRAY_BUFFER, resource_manager->m_mesh_2d->vbo_position);
  glVertexAttribPointer(g_shader->position_attr_loc, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
  glEnableVertexAttribArray(g_shader->position_attr_loc);
	glDisable(GL_DEPTH_TEST);
	CHECK_GL_ERRORS;
}

void GuiRenderer::onDisable() {
  glDisableVertexAttribArray(g_shader->position_attr_loc);
	CHECK_GL_ERRORS;
}
