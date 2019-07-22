#pragma once
#include "GameObject.hpp"
#include "../../renderers/includes/MasterRenderer.hpp"
#include "../../renderers/includes/ShadowRenderer.hpp"
#include "../../renderers/includes/NormalRenderer.hpp"
#include <memory>

class MeshObject : public GameObject {
public:
  MeshObject(const ObjectType &type, const glm::vec3 &pos);
  MeshObject(const ObjectType &type, const glm::vec3 &pos, const glm::vec3 &rot);
	virtual void render(MasterRenderer *renderer) = 0;
  virtual void render(ShadowRenderer *renderer) = 0;
  virtual void tick() override;
};


class MeshInstance : public MeshObject {
public:
  MeshInstance(const ObjectType &type, const glm::vec3 &pos);
  MeshInstance(const ObjectType &type, const glm::vec3 &pos, const glm::vec3 &rot);
  virtual void getNodes(std::vector<std::unique_ptr<GeometryNode>> &ret_nodes) = 0;
  virtual void render(MasterRenderer *renderer) override;
  virtual void render(ShadowRenderer *renderer) override;
protected:
  static void getNodes(SceneNode *model,
    std::vector<std::unique_ptr<GeometryNode>> &ret_nodes);
};


class MeshGroup : public MeshObject {
public:
  MeshGroup(const ObjectType &type, const glm::vec3 &pos, MeshInstance *instance);
  virtual void render(MasterRenderer *renderer) override;
  virtual void render(ShadowRenderer *renderer) override;
protected:
  std::unique_ptr<MeshInstance> instance;
  int data_count;
  GLuint pos_offset;
};


class MeshGroups : public MeshObject {
public:
  MeshGroups(const ObjectType &type, const glm::vec3 &pos);
  virtual void render(MasterRenderer *renderer) override;
  virtual void render(ShadowRenderer *renderer) override;
  virtual void render(NormalRenderer *renderer);
protected:
  std::vector<std::unique_ptr<MeshInstance>> instance_list;
  std::vector<int> data_count_list;
  std::vector<GLuint> pos_offset_list;
};
