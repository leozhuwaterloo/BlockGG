#pragma once
#include "MeshObjects.hpp"
#include "ColoredParticleEmitter.hpp"

class GunInstance : public MeshInstance {
public:
  GunInstance(const ObjectType &type, const glm::vec3 &pos, const float &shoot_cooldown);
  GunInstance(const ObjectType &type, const glm::vec3 &pos, const glm::vec3 &rot, const float &shoot_cooldown);
  bool attached;
  float shoot_cooldown, init_shoot_cooldown;
  virtual void shoot() = 0;
};


class GunSMG : public GunInstance {
public:
  GunSMG(const glm::vec3 &pos);
  GunSMG(const glm::vec3 &pos, const glm::vec3 &rot);
  virtual ~GunSMG();
  virtual void tick() override;
  virtual void shoot() override;
  virtual void getNodes(std::vector<std::unique_ptr<GeometryNode>> &ret_nodes) override;
};
