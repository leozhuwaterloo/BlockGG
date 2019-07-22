#pragma once
#include "../../shaders/includes/Shader.hpp"
#include "../../managers/includes/ResourceManager.hpp"

class Renderer {
public:
  Renderer(Shader *shader);
  virtual ~Renderer() = 0;

  virtual void enable();
  virtual void disable();
  virtual void onEnable() = 0;
  virtual void onDisable() = 0;
  Shader *shader;
protected:
  ResourceManager *resource_manager;
};
