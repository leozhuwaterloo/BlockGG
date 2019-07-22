#pragma once
#include "Particles.hpp"

class ColoredParticle : public ParticleInstance {
public:
  ColoredParticle(const float &life_time, const glm::vec4 &color,
    const glm::vec3 &scale);
  virtual ~ColoredParticle();
  virtual void pre_render(ParticleRenderer *renderer) override;
protected:
  glm::vec4 color;
};
