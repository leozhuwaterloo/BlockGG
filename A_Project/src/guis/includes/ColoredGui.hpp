#pragma once
#include "GuiElement.hpp"

class ColoredGui : public GuiElement {
public:
  ColoredGui(const glm::vec2 &pos, const glm::vec2 &size,
    const glm::vec4 &color);
  ColoredGui(const ElementType &type, const glm::vec2 &pos,
    const glm::vec2 &size, const glm::vec4 &color);
  virtual ~ColoredGui();

  virtual void draw(GuiRenderer *renderer) override;
  glm::vec4 color;
};
