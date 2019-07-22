#include "./includes/Blocks.hpp"

GroundEarthBlock::GroundEarthBlock(const glm::vec3 &pos)
  : MeshInstance(ObjectType::GroundEarthBlock, pos) {}
GroundEarthBlock::~GroundEarthBlock() {}
void GroundEarthBlock::getNodes(std::vector<std::unique_ptr<GeometryNode>> &ret_nodes) {
  MeshInstance::getNodes(resource_manager->model_map.at("ground_earth").get(), ret_nodes);
}


GroundSnowBlock::GroundSnowBlock(const glm::vec3 &pos)
  : MeshInstance(ObjectType::GroundSnowBlock, pos) {}
GroundSnowBlock::~GroundSnowBlock() {}
void GroundSnowBlock::getNodes(std::vector<std::unique_ptr<GeometryNode>> &ret_nodes) {
  MeshInstance::getNodes(resource_manager->model_map.at("ground_snow").get(), ret_nodes);
}


Stone6Block::Stone6Block(const glm::vec3 &pos)
  : MeshInstance(ObjectType::Stone6Block, pos) {}
Stone6Block::~Stone6Block() {}
void Stone6Block::getNodes(std::vector<std::unique_ptr<GeometryNode>> &ret_nodes) {
  MeshInstance::getNodes(resource_manager->model_map.at("stone_6").get(), ret_nodes);
}

Stone4Block::Stone4Block(const glm::vec3 &pos)
  : MeshInstance(ObjectType::Stone4Block, pos) {}
Stone4Block::~Stone4Block() {}
void Stone4Block::getNodes(std::vector<std::unique_ptr<GeometryNode>> &ret_nodes) {
  MeshInstance::getNodes(resource_manager->model_map.at("stone_4").get(), ret_nodes);
}
