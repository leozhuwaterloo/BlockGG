#pragma once
#include "GuiElement.hpp"

class StaticTexturedGui : public GuiElement {
public:
  StaticTexturedGui(const glm::vec2 &pos, const glm::vec2 &size,
    const GLuint &texture);
  virtual ~StaticTexturedGui();
  virtual void draw(GuiRenderer *renderer) override;
  GLuint texture;
};
