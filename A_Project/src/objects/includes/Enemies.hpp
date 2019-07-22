#pragma once
#include "MeshObjects.hpp"
#include "Collidable.hpp"
#include "Particles.hpp"

class Enemy : public MeshInstance, public Collidable {
public:
  Enemy(const ObjectType &type, const glm::vec3 &pos, const glm::vec3 &rot,
    const glm::vec3 &bound, const float &score);
  glm::vec3 vel, acc, bound, dis;
  float elapsed_time;
  float score;
  virtual void tick() override;
  virtual void kill(const bool &add_score);
  virtual void kill();
  virtual Bound getBound() override;
};


// -----------------------------------
class Slime : public Enemy {
public:
  Slime(const glm::vec3 &pos, const glm::vec3 &rot);
  ~Slime();
  virtual void tick() override;
  virtual void kill(const bool &add_score) override;
  virtual void getNodes(std::vector<std::unique_ptr<GeometryNode>> &ret_nodes) override;
  float jump_interval;
  ALuint sound_source;
  float effect_sound;
};
