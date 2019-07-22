#include "./includes/Enviroments.hpp"

Tree1::Tree1(const glm::vec3 &pos)
  : MeshInstance(ObjectType::Tree1, pos) {}
Tree1::~Tree1() {}
void Tree1::getNodes(std::vector<std::unique_ptr<GeometryNode>> &ret_nodes) {
  MeshInstance::getNodes(resource_manager->model_map.at("tree1").get(), ret_nodes);
}



Grass::Grass(const glm::vec3 &pos)
  : MeshInstance(ObjectType::Grass, pos) {}
Grass::~Grass() {}
void Grass::getNodes(std::vector<std::unique_ptr<GeometryNode>> &ret_nodes) {
  MeshInstance::getNodes(resource_manager->model_map.at("grass").get(), ret_nodes);
}
