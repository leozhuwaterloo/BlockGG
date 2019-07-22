#pragma once
#include "GameObject.hpp"
#include "../../renderers/includes/ParticleRenderer.hpp"

class Particle : public GameObject {
public:
  Particle(const ObjectType &type, const glm::vec3 &pos, const float &life_time);
  float life_time, elapsed_time;
};

class ParticleInstance : public Particle {
public:
  ParticleInstance(const ObjectType &type, const float &life_time);
  ParticleInstance(const ObjectType &type, const float &life_time, const glm::vec3 &scale);
  virtual void getModelView(const glm::vec3 &position, glm::mat4 &model_view);
  virtual void tick() override;
  virtual void pre_render(ParticleRenderer *renderer) = 0;
  glm::vec3 scale, vel, acc;
};

class ParticleSystem : public Particle {
public:
  ParticleSystem(const ObjectType &type, const glm::vec3 &pos, const float &life_time);
  ~ParticleSystem();
  virtual void render(ParticleRenderer *renderer);
  virtual void tick() override;
  virtual void reset(ParticleInstance *instance) = 0;
  virtual void resetAll();
  virtual void enable();
  virtual void disable();
protected:
  bool enabled;
  std::vector<glm::mat4> model_view_data;
  std::list<std::unique_ptr<ParticleInstance>> instances;
  GLuint model_views;
};
