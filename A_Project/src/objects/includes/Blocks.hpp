#pragma once
#include "MeshObjects.hpp"

class GroundEarthBlock : public MeshInstance {
public:
  GroundEarthBlock(const glm::vec3 &pos);
  virtual ~GroundEarthBlock();
  virtual void getNodes(std::vector<std::unique_ptr<GeometryNode>> &ret_nodes) override;
};


class GroundSnowBlock : public MeshInstance {
public:
  GroundSnowBlock(const glm::vec3 &pos);
  virtual ~GroundSnowBlock();
  virtual void getNodes(std::vector<std::unique_ptr<GeometryNode>> &ret_nodes) override;
};

class Stone6Block : public MeshInstance {
public:
  Stone6Block(const glm::vec3 &pos);
  virtual ~Stone6Block();
  virtual void getNodes(std::vector<std::unique_ptr<GeometryNode>> &ret_nodes) override;
};

class Stone4Block : public MeshInstance {
public:
  Stone4Block(const glm::vec3 &pos);
  virtual ~Stone4Block();
  virtual void getNodes(std::vector<std::unique_ptr<GeometryNode>> &ret_nodes) override;
};
