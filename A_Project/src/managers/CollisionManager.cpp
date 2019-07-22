#include "./includes/CollisionManager.hpp"

#include "../objects/includes/Bullet.hpp"
#include "../objects/includes/Enemies.hpp"
#include "./includes/GameManager.hpp"
#include <glm/gtx/string_cast.hpp>
#include <unordered_set>

CollisionManager::CollisionManager()
  : resource_manager(ResourceManager::getInstance()) {}
CollisionManager::~CollisionManager() {}


void CollisionManager::tick() {
  if (!resource_manager->game->command_manager->dynamic_collision) return;
  std::unordered_set<Enemy*> need_kill;
  bool player_damaged(false);
  for (GameObject *object : resource_manager->game->objects) {
    if (object->type == ObjectType::Slime) {
      Enemy *enemy = static_cast<Enemy*>(object);
      for (GameObject *obj : resource_manager->game->objects) {
        if (obj == object) continue;
        if (obj->type == ObjectType::Bullet) {
          Bullet *bullet = static_cast<Bullet*>(obj);
          if (bullet->collide(enemy)) {
            need_kill.insert(enemy);
            break;
          }
        }
      }
      if (!player_damaged && enemy->collide(resource_manager->game->player)) {
        player_damaged = true;
        resource_manager->game->player->damage(5.0f);
      }
    }
  }
  for (const auto &it : need_kill) {
    it->kill();
  }
  if (player_damaged) {

  }
}
