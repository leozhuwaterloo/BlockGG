#include "./includes/GameObject.hpp"
#include "../managers/includes/GameManager.hpp"

static unsigned int _id = 0;

GameObject::GameObject(const ObjectType &type, const glm::vec3 &pos)
  : GameObject(type, pos, glm::vec3(0)) {}

GameObject::GameObject(const ObjectType &type, const glm::vec3 &pos, const glm::vec3 &rot)
  : id(++_id),
    type(type),
    pos(pos),
    rot(rot),
    next(nullptr),
    parent(nullptr),
    resource_manager(ResourceManager::getInstance()) {}

GameObject::~GameObject() {}


void GameObject::checkCollideTerrain(const glm::vec3 &bound, glm::vec3 &dis) {
  if (!resource_manager->game->command_manager->static_collision) return;
  dis *= 2.0f;
  Terrain *terrain = resource_manager->game->terrain;
  float x_dir = dis.x > 0 ? 1.0f : -1.0f;
  float z_dir = dis.z > 0 ? 1.0f : -1.0f;
  if (dis.x != 0 && terrain->getHeight(pos.x + dis.x + x_dir * bound.x, pos.z) +
    bound.y > pos.y) dis.x = 0;
  if (dis.z != 0 && terrain->getHeight(pos.x, pos.z + dis.z + z_dir * bound.z) +
    bound.y > pos.y) dis.z = 0;
  if (dis.x != 0 && dis.y != 0 &&
    terrain->getHeight(pos.x + dis.x + x_dir * bound.x, pos.z + dis.z + z_dir * bound.z) +
    bound.y > pos.y) dis.x = dis.z = 0;

  if (terrain->getHeight(pos.x + dis.x, pos.z + dis.z) +
    bound.y > pos.y + dis.y) dis.y = 0;
  dis /= 2.0f;
}
