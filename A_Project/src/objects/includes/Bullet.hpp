#pragma once
#include "MeshObjects.hpp"
#include "Collidable.hpp"

class Bullet : public MeshInstance, public Collidable {
public:
  Bullet(const glm::vec3 &pos, const glm::vec3 &rot, const glm::vec3 &vel);
  glm::vec3 vel, acc, bound;
  virtual ~Bullet();
  virtual void tick() override;
  virtual void render(MasterRenderer *renderer) override;
  virtual void getNodes(std::vector<std::unique_ptr<GeometryNode>> &ret_nodes) override;
  virtual Bound getBound() override;
protected:
  float elapsed_time;
};
