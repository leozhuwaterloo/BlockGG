#include "./includes/FadeColoredGui.hpp"
#include "../managers/includes/GameManager.hpp"

FadeColoredGui::FadeColoredGui(const glm::vec2 &pos, const glm::vec2 &size,
  const glm::vec4 &color, const float &fade_speed)
  : ColoredGui(ElementType::FadeColoredGui, pos, size, color),
    fade_speed(fade_speed) {}
FadeColoredGui::~FadeColoredGui() {}


void FadeColoredGui::tick() {
  color.a -= fade_speed;
  if (color.a < 0.0f) {
    resource_manager->game->remove(this);
  }
}
