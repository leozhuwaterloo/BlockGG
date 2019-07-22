#pragma once
#include "Particles.hpp"

class ColoredParticleEmitter : public ParticleSystem {
public:
  ColoredParticleEmitter(const glm::vec3 &pos,
    const unsigned int &count, const float &speed, const float &life_time,
    const float &particle_life_time, const glm::vec4 &color, const bool &blast);
  ColoredParticleEmitter(const glm::vec3 &pos,
    const unsigned int &count, const float &speed, const float &life_time,
    const float &particle_life_time, const glm::vec4 &color, const bool &blast,
    const glm::vec3 &dir);
  ColoredParticleEmitter(const glm::vec3 &pos,
    const unsigned int &count, const float &speed, const float &life_time,
    const float &particle_life_time, const glm::vec4 &color, const bool &blast,
    const glm::vec3 &dir, const glm::vec3 &scale);
  virtual ~ColoredParticleEmitter();

  virtual void reset(ParticleInstance *instance) override;
  unsigned int count;
  float speed;
  bool blast;
  glm::vec3 dir;
};
