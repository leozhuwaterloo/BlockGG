#pragma once
#include "GameObject.hpp"
#include "Guns.hpp"
#include "../../managers/includes/ResourceManager.hpp"
#include "Collidable.hpp"
#include "../../guis/includes/ColoredGui.hpp"


class Player : public GameObject, public Collidable {
public:
  Player(const glm::vec3 &pos);
  Player(const glm::vec3 &pos, const glm::vec3 &rot);
  virtual ~Player();

  glm::vec3 forward, up, right;
  glm::vec3 vel, acc;
  glm::vec3 gravity;
  float speed, rot_speed, jump_power;
  bool compute_view;
  float shoot_cooldown, init_shoot_cooldown;
  int jump_count, init_jump_count;
  float damage_cooldown, init_damage_cooldown;
  glm::vec3 bound;
  virtual void tick() override;
  void onMouseMove(const double &moved_x, const double &moved_y);
  void jump();
  void damage(const float &damage);
  void respawn();
  void setResourceManager(ResourceManager *resource_manager);
  virtual Bound getBound() override;
  float health, init_health;
  float score, pre_score;
protected:
  GunInstance *gun;
  ColoredGui *hp_bar;
  glm::vec2 hp_bar_init_pos, hp_bar_init_size;
  glm::vec3 init_forward, init_up;
  void computeDirections();
};
