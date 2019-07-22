#pragma once
#include "ColoredGui.hpp"

class FadeColoredGui : public ColoredGui {
public:
  FadeColoredGui(const glm::vec2 &pos, const glm::vec2 &size,
    const glm::vec4 &color, const float &fade_speed);
  virtual ~FadeColoredGui();

  virtual void tick() override;
  float fade_speed;
};
