#include "./includes/ColoredGui.hpp"
#include <glm/gtc/type_ptr.hpp>


ColoredGui::ColoredGui(const ElementType &type, const glm::vec2 &pos,
  const glm::vec2 &size, const glm::vec4 &color)
  : GuiElement(type, pos, size),
    color(color) {}
ColoredGui::ColoredGui(const glm::vec2 &pos, const glm::vec2 &size,
  const glm::vec4 &color)
  : ColoredGui(ElementType::ColoredGui, pos, size, color) {}
ColoredGui::~ColoredGui() {}


void ColoredGui::draw(GuiRenderer *renderer) {
  glUniform1i(renderer->g_shader->using_texture_loc, 0);
  glUniform4fv(renderer->g_shader->color_loc, 1, glm::value_ptr(color));
  const BatchInfo &batchInfo = resource_manager->batch_info_map.at("quad");
  glUniformMatrix4fv(renderer->g_shader->transformation_loc, 1, GL_FALSE,
    glm::value_ptr(m_transformation));
  glDrawArrays(GL_TRIANGLE_STRIP, batchInfo.startIndex, batchInfo.numIndices);
}
