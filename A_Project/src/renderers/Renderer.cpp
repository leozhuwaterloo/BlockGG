#include "./includes/Renderer.hpp"

Renderer::Renderer(Shader *shader)
  : shader(shader),
    resource_manager(ResourceManager::getInstance()) {}

Renderer::~Renderer() {}


void Renderer::enable() {
  shader->enable();
  onEnable();
}


void Renderer::disable() {
  onDisable();
  shader->disable();
}
