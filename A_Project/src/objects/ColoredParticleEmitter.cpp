#include "./includes/ColoredParticleEmitter.hpp"
#include "./includes/ColoredParticle.hpp"
#include "../utils/includes/CommonUtils.hpp"



ColoredParticleEmitter::ColoredParticleEmitter(const glm::vec3 &pos,
  const unsigned int &count, const float &speed, const float &life_time,
  const float &particle_life_time, const glm::vec4 &color, const bool &blast,
  const glm::vec3 &dir, const glm::vec3 &scale)
  : ParticleSystem(ObjectType::ColoredParticleEmitter, pos, life_time),
    count(count),
    speed(speed),
    blast(blast),
    dir(dir)
{
  for (int i = 0; i < count; ++i) {
    instances.push_back(std::move(std::unique_ptr<ParticleInstance>(
      new ColoredParticle(particle_life_time, color, scale))));
  }
}
ColoredParticleEmitter::ColoredParticleEmitter(const glm::vec3 &pos,
  const unsigned int &count, const float &speed, const float &life_time,
  const float &particle_life_time, const glm::vec4 &color, const bool &blast,
  const glm::vec3 &dir)
  : ColoredParticleEmitter(pos, count, speed, life_time, particle_life_time,
    color, blast, dir, glm::vec3(0.1f)) {}
ColoredParticleEmitter::ColoredParticleEmitter(const glm::vec3 &pos,
  const unsigned int &count, const float &speed, const float &life_time,
  const float &particle_life_time, const glm::vec4 &color, const bool &blast)
  : ColoredParticleEmitter(pos, count, speed, life_time, particle_life_time,
    color, blast, glm::vec3(0.0f), glm::vec3(0.1f)) {}

ColoredParticleEmitter::~ColoredParticleEmitter() {}


void ColoredParticleEmitter::reset(ParticleInstance *instance) {
  instance->pos = glm::vec3(0.0f);
  instance->vel = dir + CommonUtils::getRandomVec3() * 0.05f * speed;
  if (blast) instance->elapsed_time = 0.0;
  else instance->elapsed_time = rand() % (int)instance->life_time;
}
