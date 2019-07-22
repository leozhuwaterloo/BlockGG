#include "./includes/Enemies.hpp"
#include "../managers/includes/GameManager.hpp"
#include "../utils/includes/CommonUtils.hpp"
#include "./includes/ColoredParticleEmitter.hpp"
#include "./includes/TempSound.hpp"

Enemy::Enemy(const ObjectType &type, const glm::vec3 &pos, const glm::vec3 &rot,
  const glm::vec3 &bound, const float &score)
  : MeshInstance(type, pos, rot),
    vel(glm::vec3(0.0f)),
    bound(bound),
    score(score) {}

void Enemy::tick() {
  float time_delta = resource_manager->game->time_delta;
  acc = resource_manager->game->player->gravity;
  vel += acc * time_delta;
  dis = vel * time_delta;
  checkCollideTerrain(bound, dis);
  pos += dis;
  elapsed_time += time_delta;
}

void Enemy::kill(const bool &add_score) {
  resource_manager->game->remove(this);
  if (add_score) resource_manager->game->player->score += score;
}

void Enemy::kill() {
  kill(true);
}

Bound Enemy::getBound() {
  Bound b(pos - bound, pos + bound);
  return b;
}

// -------------------------------------------------------------
Slime::Slime(const glm::vec3 &pos, const glm::vec3 &rot)
  : Enemy(ObjectType::Slime, pos, rot, glm::vec3(0.5f, 1.0f, 0.5f), 1.0f),
    jump_interval(CommonUtils::randFloat() * 400.0f + 100.0f),
    effect_sound(0.5f)
{
  alGenSources(1, &sound_source);
  alSourcef(sound_source, AL_PITCH, 1.0f);
  alSourcef(sound_source, AL_GAIN, effect_sound *
    resource_manager->game->command_manager->sound_effect);
  alSourcei(sound_source, AL_LOOPING, AL_FALSE);
  alSourcei(sound_source, AL_BUFFER, resource_manager->sound_map.at("slime_jump"));
}
Slime::~Slime() {
  alDeleteSources(1, &sound_source);
}

void Slime::tick() {
  Enemy::tick();
  if (dis.y == 0) {
    vel.x *= 0.9f;
    vel.z *= 0.9f;
  }
  glm::vec3 player_pos = resource_manager->game->player->pos;
  rot.y = glm::degrees(glm::atan(pos.x - player_pos.x, pos.z - player_pos.z)) + 90.0f;
  if (elapsed_time > jump_interval) {
    vel = player_pos - pos;
    jump_interval = CommonUtils::randFloat() * 400.0f + 100.0f;
    vel.x = glm::clamp(vel.x, -0.06f, 0.06f);
    vel.z = glm::clamp(vel.z, -0.06f, 0.06f);
    vel.y = CommonUtils::randFloat() * 0.05f + 0.05f;
    elapsed_time -= 200.0f;
    alSourcePlay(sound_source);
  }

  alSource3f(sound_source, AL_POSITION, pos.x, pos.y, pos.z);
  alSource3f(sound_source, AL_VELOCITY, dis.x, dis.y, dis.z);
}

void Slime::getNodes(std::vector<std::unique_ptr<GeometryNode>> &ret_nodes) {
  MeshInstance::getNodes(resource_manager->model_map.at("slime").get(), ret_nodes);
}


void Slime::kill(const bool &add_score) {
  if (add_score) {
    ColoredParticleEmitter *death_particle = new ColoredParticleEmitter(
      pos, 80, 10.0f, 80.0f, 80.0f, glm::vec4(0.0f, 1.0f, 0.0f, 0.6f), true);
    death_particle->pos = pos;
    death_particle->enable();
    resource_manager->game->add(new TempSound(pos,
      resource_manager->sound_map.at("slime_death"), 1.0f, 5.0f));
  }
  Enemy::kill(add_score);
}
