#include "./includes/EnemySpawner.hpp"
#include "./includes/Enemies.hpp"
#include "../managers/includes/GameManager.hpp"
#include "../utils/includes/CommonUtils.hpp"

EnemySpawner::EnemySpawner()
  : GameObject(ObjectType::EnemySpawner, glm::vec3(0.0f)),
    elapsed_time(0.0f),
    spawn_interval(200.0f) {}

EnemySpawner::~EnemySpawner() {}


void EnemySpawner::tick() {
  GameManager *game = resource_manager->game;
  elapsed_time += game->time_delta;
  if (elapsed_time > spawn_interval) {
    elapsed_time -= spawn_interval;
    if (game->objects.size < 100) {
      float rand_x = (CommonUtils::randFloat() - 0.5f) * 40.0f;
      float rand_z = (CommonUtils::randFloat() - 0.5f) * 40.0f;
      rand_x = (rand_x > 0 ? 1 : -1) * 5.0 + rand_x;
      rand_z = (rand_z > 0 ? 1 : -1) * 5.0 + rand_z;
      glm::vec3 spawn_pos = game->player->pos + glm::vec3(rand_x, 0.0f, rand_z);
      spawn_pos.y = game->terrain->getHeight(spawn_pos.x, spawn_pos.z) + 4.0f;
      game->add(new Slime(spawn_pos, glm::vec3(0.0f,
        CommonUtils::randFloat() * 360.0f, 0.0f)));
    }
  }
}
