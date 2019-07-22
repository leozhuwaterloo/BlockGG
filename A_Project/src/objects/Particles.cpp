#include "./includes/Particles.hpp"
#include "cs488-framework/GlErrorCheck.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "../managers/includes/GameManager.hpp"
#include "../managers/includes/LogManager.hpp"
#include "../utils/includes/CommonUtils.hpp"
static LogManager *logger = LogManager::getInstance();

Particle::Particle(const ObjectType &type, const glm::vec3 &pos, const float &life_time)
  : GameObject(type, pos),
    life_time(life_time),
    elapsed_time(0.0f) {}


// ---------------------------------------------------------------
ParticleInstance::ParticleInstance(const ObjectType &type, const float &life_time,
  const glm::vec3 &scale)
  : Particle(type, glm::vec3(0.0f), life_time),
    vel(glm::vec3(0.0f)),
    acc(glm::vec3(0.0f)),
    scale(scale) {}
ParticleInstance::ParticleInstance(const ObjectType &type, const float &life_time)
  : ParticleInstance(type, life_time, glm::vec3(0.1f)) {}


void ParticleInstance::getModelView(const glm::vec3 &position, glm::mat4 &model_view) {
  model_view = resource_manager->m_view * glm::translate(glm::mat4(1.0f), position + pos);
  for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < 3; ++j) {
      model_view[i][j] = (i == j) ? 1.0f : 0.0f;
    }
  }
  model_view = glm::scale(model_view, scale);
}

void ParticleInstance::tick() {
  float time_delta = resource_manager->game->time_delta;
  vel += acc * time_delta;
  pos += vel * time_delta;
  elapsed_time += time_delta;
}

// ----------------------------------------------------------------

ParticleSystem::ParticleSystem(const ObjectType &type, const glm::vec3 &pos,
  const float &life_time)
  : Particle(type, pos, life_time),
    enabled(false)
{
  glGenBuffers(1, &model_views);
}
ParticleSystem::~ParticleSystem() {
  glDeleteBuffers(1, &model_views);
}

void ParticleSystem::resetAll() {
  for (const auto &instance : instances) {
    reset(instance.get());
  }
}

void ParticleSystem::enable() {
  if (!enabled) {
    enabled = true;
    resetAll();
    resource_manager->game->add(this);
  } else {
    logger->log(LogLevel::Error, CommonUtils::format(
      "Enabled particle system %d more than once", id));
  }
}

void ParticleSystem::disable() {
  if (enabled) {
    enabled = false;
    resource_manager->game->remove(this, false);
  } else {
    logger->log(LogLevel::Error, CommonUtils::format(
      "Disabled particle system %d more than noce", id));
  }
}

void ParticleSystem::tick() {
  elapsed_time += resource_manager->game->time_delta;
  if(life_time > 0.0f && elapsed_time > life_time) {
    resource_manager->game->remove(this);
    return;
  }
  model_view_data.clear();
  std::list<std::unique_ptr<ParticleInstance>>::iterator it = instances.begin();
  while(it != instances.end()) {
    ParticleInstance *instance = (*it).get();
    instance->tick();
    if (instance->elapsed_time > instance->life_time) {
      reset(instance);
    } else {
      glm::mat4 model_view;
      instance->getModelView(pos, model_view);
      model_view_data.push_back(model_view);
    }
    ++it;
  }
  glBindBuffer(GL_ARRAY_BUFFER, model_views);
  glBufferData(GL_ARRAY_BUFFER, model_view_data.size() * sizeof(glm::mat4),
    &(model_view_data[0][0].x), GL_STATIC_DRAW);
  CHECK_GL_ERRORS;
}


void ParticleSystem::render(ParticleRenderer *renderer) {
  instances.front()->pre_render(renderer);
  glBindBuffer(GL_ARRAY_BUFFER, model_views);
  unsigned int l_byte = sizeof(GLfloat);
  for(int i = 0; i < 4; ++i) {
    glEnableVertexAttribArray(renderer->p_shader->model_view_attr_loc + i);
    glVertexAttribPointer(renderer->p_shader->model_view_attr_loc + i, 4, GL_FLOAT,
      GL_FALSE, l_byte* 16, (void *)(intptr_t)(l_byte * i * 4));
  }
  const BatchInfo &batchInfo = resource_manager->batch_info_map.at("quad");
  glDrawArraysInstanced(GL_TRIANGLE_STRIP, batchInfo.startIndex, batchInfo.numIndices, model_view_data.size());
  CHECK_GL_ERRORS;
}
