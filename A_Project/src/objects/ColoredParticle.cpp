#include "./includes/ColoredParticle.hpp"
#include <glm/gtc/type_ptr.hpp>

ColoredParticle::ColoredParticle(const float &life_time, const glm::vec4 &color,
  const glm::vec3 &scale)
  : ParticleInstance(ObjectType::ColoredParticle, life_time, scale),
    color(color) {}

ColoredParticle::~ColoredParticle() {}

void ColoredParticle::pre_render(ParticleRenderer *renderer) {
  glUniform1i(renderer->p_shader->using_texture_loc, 0);
  glUniform4fv(renderer->p_shader->color_loc, 1, glm::value_ptr(color));
}
