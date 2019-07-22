#include "./includes/StaticTexturedGui.hpp"
#include <glm/gtc/type_ptr.hpp>

StaticTexturedGui::StaticTexturedGui(const glm::vec2 &pos, const glm::vec2 &size,
  const GLuint &texture)
  : GuiElement(ElementType::StaticTexturedGui, pos, size),
    texture(texture) {}

StaticTexturedGui::~StaticTexturedGui() {}


void StaticTexturedGui::draw(GuiRenderer *renderer) {
  glUniform1i(renderer->g_shader->using_texture_loc, 1);
  glUniform4fv(renderer->g_shader->color_loc, 1, glm::value_ptr(glm::vec4(1.0f)));
  const BatchInfo &batchInfo = resource_manager->batch_info_map.at("quad");
  glBindTexture(GL_TEXTURE_2D, texture);
  glUniformMatrix4fv(renderer->g_shader->transformation_loc, 1, GL_FALSE,
    glm::value_ptr(m_transformation));
  glDrawArrays(GL_TRIANGLE_STRIP, batchInfo.startIndex, batchInfo.numIndices);
}
