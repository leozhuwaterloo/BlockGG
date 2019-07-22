#include "./includes/Player.hpp"
#include "../managers/includes/GameManager.hpp"
#include "../managers/includes/LogManager.hpp"
#include "../utils/includes/CommonUtils.hpp"
#include <glm/gtx/rotate_vector.hpp>
#include "./includes/Guns.hpp"
#include "../guis/includes/FadeColoredGui.hpp"

static LogManager *logger = LogManager::getInstance();


Player::Player(const glm::vec3 &pos): Player(pos, glm::vec3(0)) {}
Player::Player(const glm::vec3 &pos, const glm::vec3 &rot)
  : GameObject(ObjectType::Player, pos, rot),
    forward(glm::vec3(0.0f, 0.0f, -1.0f)),
    up(glm::vec3(0.0f, 1.0f, 0.0f)),
    right(glm::cross(forward, up)),
    init_forward(forward),
    init_up(up),
    speed(0.01f),
    rot_speed(0.05f),
    jump_power(0.1f),
    compute_view(false),
    vel(glm::vec3(0)),
    gravity(glm::vec3(0.0f, -0.0015f, 0.0f)),
    shoot_cooldown(3.0f),
    init_shoot_cooldown(shoot_cooldown),
    jump_count(5),
    init_jump_count(jump_count),
    damage_cooldown(50.0f),
    init_damage_cooldown(damage_cooldown),
    health(100.0f),
    init_health(health),
    score(0.0f),
    pre_score(0.0f),
    bound(glm::vec3(0.25f, 2.0f, 0.25f)) {}

Player::~Player() {}

void Player::tick() {
  // To do make this time based
  bool *key_pressed = resource_manager->game->event_manager->key_pressed;
  float time_delta = resource_manager->game->time_delta;
  glm::vec3 forward_unit = glm::normalize(glm::vec3(forward.x, 0.0f, forward.z));
  glm::vec3 right_unit = glm::normalize(glm::vec3(right.x, 0.0f, right.z));
  glm::vec3 up_unit = glm::cross(forward_unit, -right_unit);
  bool god_mode = resource_manager->game->command_manager->god_mode;
  acc = god_mode ? glm::vec3(0.0f) : gravity;
  float friction = 1 - 0.1f * resource_manager->game->command_manager->friction;
  vel = god_mode ? glm::vec3(0.0f) : glm::vec3(vel.x * friction, vel.y, vel.z * friction);
  bool moved(false);
  if (key_pressed[0]) vel += speed * forward_unit;
  if (key_pressed[1]) vel += speed * right_unit;
  if (key_pressed[2]) vel -= speed * forward_unit;
  if (key_pressed[3]) vel -= speed * right_unit;
  if (god_mode && key_pressed[5]) vel += speed * up_unit;
  if (god_mode && key_pressed[6]) vel -= speed * up_unit;
  float limit = speed * 10.0f;
  vel.x = glm::clamp(vel.x, -limit, limit);
  vel.z = glm::clamp(vel.z, -limit, limit);
  vel += time_delta * acc;
  glm::vec3 dis = time_delta * vel;
  checkCollideTerrain(bound, dis);
  pos += dis;
  if (dis.y == 0) {
    jump_count = init_jump_count;
    vel.y = 0.0f;
  }

  if (compute_view) {
    computeDirections();
  }

  gun->tick();
  if (key_pressed[4] && shoot_cooldown < 0.0f) {
    shoot_cooldown = init_shoot_cooldown;
    gun->shoot();
  }
  alListener3f(AL_POSITION, pos.x, pos.y, pos.z);
  alListener3f(AL_VELOCITY, dis.x, dis.y, dis.z);
  ALfloat ori[] = {forward.x, forward.y, forward.z, up.x, up.y, up.z};
  alListenerfv(AL_ORIENTATION, ori);
  resource_manager->initViewMatrix();
  if (shoot_cooldown > 0.0f) shoot_cooldown -= time_delta;
  if (damage_cooldown > 0.0f) damage_cooldown -= time_delta;
}

void Player::damage(const float &damage) {
  if (damage_cooldown < 0.0f) {
    resource_manager->game->add(new FadeColoredGui(glm::vec2(0.0f),
      glm::vec2(1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 0.3f), 0.003f));
    damage_cooldown = init_damage_cooldown;
    health -= damage;
    logger->log(LogLevel::Debug, CommonUtils::format(
      "Player took %.2f damange; Remaining: %.2f", damage, health));

    float changed = hp_bar_init_size.x * (health / init_health);
    hp_bar->size.x = changed;
    hp_bar->pos.x = hp_bar_init_pos.x - (hp_bar_init_size.x - changed);
    hp_bar->computeTransformation();

    if (health < 0.0f) {
      resource_manager->game->restart();
    }
  }
}

void Player::onMouseMove(const double &moved_x, const double &moved_y) {
  if (moved_x || moved_y) {
    rot[1] += rot_speed * moved_x;
    rot[0] += rot_speed * moved_y;
    rot[0] = glm::clamp(rot[0], -89.0f, 89.0f);
    compute_view = true;
  }
}

void Player::jump() {
  if (jump_count > 0) {
    --jump_count;
    logger->log(LogLevel::Debug,
      CommonUtils::format("Jumping (%d jumps remaining)", jump_count));
    vel.y = jump_power;
  } else {
    logger->log(LogLevel::Debug, "Failed to jump (Reason: no jumps left)");
  }
}

void Player::computeDirections() {
  float rot_x_rad = glm::radians(rot[0]), rot_y_rad = glm::radians(rot[1]);
  forward = glm::rotateX(init_forward, rot_x_rad);
  up = glm::rotateX(init_up, rot_x_rad);
  forward = glm::rotateY(forward, rot_y_rad);
  up = glm::rotateY(up, rot_y_rad);
  right = glm::cross(forward, up);
}

void Player::setResourceManager(ResourceManager *resource_manager) {
  this->resource_manager = resource_manager;
  gun = new GunSMG(glm::vec3(0.0f));
  gun->attached = true;
  resource_manager->game->add(gun);

  hp_bar = new ColoredGui(glm::vec2(0.50f, -0.8389f), glm::vec2(0.13f, 0.018f),
    glm::vec4(0.996f, 0.3765f, 0.5647f, 1.0f));
  hp_bar_init_pos = hp_bar->pos;
  hp_bar_init_size = hp_bar->size;
  resource_manager->game->add(hp_bar);
}

void Player::respawn() {
  float x = CommonUtils::randFloat() * 20000.0f - 10000.0f;
  float z = CommonUtils::randFloat() * 20000.0f - 10000.0f;
  float y = resource_manager->game->terrain->getHeight(x, z) + 10.0f;
  pos = glm::vec3(x, y, z);
  vel = glm::vec3(0.0f);
  health = init_health;
  pre_score = score;
  score = 0.0f;
}

Bound Player::getBound() {
  Bound b(pos - bound, glm::vec3(pos.x + bound.x, pos.y, pos.z + bound.z));
  return b;
}
