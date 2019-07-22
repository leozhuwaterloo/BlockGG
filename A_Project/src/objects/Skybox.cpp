#include "./includes/Skybox.hpp"


Skybox::Skybox():
  GameObject(ObjectType::Skybox, glm::vec3(0.0f)) {}
Skybox::~Skybox() {}


void Skybox::tick() {}


void Skybox::render(SkyboxRenderer *renderer) {
  const BatchInfo &batchInfo = resource_manager->batch_info_map.at("cube");
  glDrawArrays(GL_TRIANGLES, batchInfo.startIndex, batchInfo.numIndices);
}
