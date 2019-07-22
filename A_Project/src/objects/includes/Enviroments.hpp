#pragma once
#include "MeshObjects.hpp"

class Tree1 : public MeshInstance {
public:
  Tree1(const glm::vec3 &pos);
  virtual ~Tree1();
  virtual void getNodes(std::vector<std::unique_ptr<GeometryNode>> &ret_nodes) override;
};

class Grass : public MeshInstance {
public:
  Grass(const glm::vec3 &pos);
  virtual ~Grass();
  virtual void getNodes(std::vector<std::unique_ptr<GeometryNode>> &ret_nodes) override;
};
