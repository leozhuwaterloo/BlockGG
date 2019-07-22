#include "./includes/Bullet.hpp"
#include "../managers/includes/GameManager.hpp"
#include <glm/gtc/type_ptr.hpp>

Bullet::Bullet(const glm::vec3 &pos, const glm::vec3 &rot, const glm::vec3 &vel)
  : MeshInstance(ObjectType::Bullet, pos, rot),
    vel(vel),
    bound(glm::vec3(0.025, 0.025, 0.05)),
    elapsed_time(0.0f) {}

Bullet::~Bullet() {}

void Bullet::getNodes(std::vector<std::unique_ptr<GeometryNode>> &ret_nodes) {
  MeshInstance::getNodes(resource_manager->model_map.at("bullet").get(), ret_nodes);
}

void Bullet::tick() {
  float time_delta = resource_manager->game->time_delta;
  pos += vel * time_delta;
  elapsed_time += time_delta;
  if (elapsed_time > 50.0f) {
    resource_manager->game->remove(this);
  }
}

void Bullet::render(MasterRenderer *renderer) {
  glUniform3fv(renderer->m_shader->ambient_intensity_loc, 1, glm::value_ptr(glm::vec3(2.0f)));
  MeshInstance::render(renderer);
  glUniform3fv(renderer->m_shader->ambient_intensity_loc, 1, glm::value_ptr(resource_manager->ambient_intensity));
}

Bound Bullet::getBound() {
  Bound b(pos - bound, pos + bound);
  return b;
}
