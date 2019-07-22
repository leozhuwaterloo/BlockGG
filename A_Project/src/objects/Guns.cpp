#include "./includes/Guns.hpp"
#include "../managers/includes/GameManager.hpp"
#include <glm/gtx/rotate_vector.hpp>
#include "./includes/Bullet.hpp"
#include "../utils/includes/CommonUtils.hpp"
#include "./includes/TempSound.hpp"

GunInstance::GunInstance(const ObjectType &type, const glm::vec3 &pos,
  const glm::vec3 &rot, const float &shoot_cooldown)
  : MeshInstance(type, pos, rot),
    attached(false),
    shoot_cooldown(shoot_cooldown),
    init_shoot_cooldown(shoot_cooldown){}
GunInstance::GunInstance(const ObjectType &type, const glm::vec3 &pos,
  const float &shoot_cooldown)
  : GunInstance(type, pos, glm::vec3(0.0f), shoot_cooldown) {}



// ----------------------------------
GunSMG::GunSMG(const glm::vec3 &pos, const glm::vec3 &rot)
  : GunInstance(ObjectType::GunSMG, pos, rot, 20.0f) {}
GunSMG::GunSMG(const glm::vec3 &pos)
  : GunSMG(pos, glm::vec3(0.0f)) {}
GunSMG::~GunSMG() {}

void GunSMG::tick() {
  if (attached) {
    Player *player = resource_manager->game->player;
    pos = player->pos + player->forward * 1.2f +
      player->right * 0.45f - player->up * 0.5f;
    if (resource_manager->game->event_manager->key_pressed[4]) {
      pos += CommonUtils::getRandomVec3() * 0.03f;
    }
    rot = player->rot;
    shoot_cooldown -= resource_manager->game->time_delta;
  }
}
void GunSMG::shoot() {
  if (shoot_cooldown < 0.0f) {
    shoot_cooldown = init_shoot_cooldown;
    Player *player = resource_manager->game->player;
    glm::vec3 forward = resource_manager->game->player->forward;
    glm::vec3 up = resource_manager->game->player->up;
    glm::vec3 right = resource_manager->game->player->right;
    glm::vec3 spawn_pos = pos + forward * 1.8f + up * 0.22f;
    forward += CommonUtils::getRandomVec3() * 0.03f;
    resource_manager->game->add(new Bullet(spawn_pos, rot, forward));
    (new ColoredParticleEmitter(
      spawn_pos - forward, 5, 1.2f, 10.0f, 10.0f, glm::vec4(0.0f, 1.0f, 1.0f, 0.4f),
      true, forward, glm::vec3(0.05f)))->enable();
    pos -= forward * 0.3f;
    resource_manager->game->add(new TempSound(spawn_pos,
      resource_manager->sound_map.at("laser_shot"), 1.0f, 0.05f));
  }
}
void GunSMG::getNodes(std::vector<std::unique_ptr<GeometryNode>> &ret_nodes) {
  MeshInstance::getNodes(resource_manager->model_map.at("gun_smg").get(), ret_nodes);
}
