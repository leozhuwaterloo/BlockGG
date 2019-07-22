#pragma once
#include "MeshObjects.hpp"
#include "../../renderers/includes/NormalRenderer.hpp"
#include "../../renderers/includes/MasterRenderer.hpp"
#include "../../renderers/includes/ShadowRenderer.hpp"

class Crate : public MeshInstance {
public:
  Crate(const glm::vec3 &pos);
  virtual ~Crate();
  virtual void render(NormalRenderer *renderer);
  virtual void render(MasterRenderer *renderer) override;
  virtual void render(ShadowRenderer *renderer) override;
  virtual void getNodes(std::vector<std::unique_ptr<GeometryNode>> &ret_nodes) override;
};
