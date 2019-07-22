#pragma once
#include "Renderer.hpp"
#include "../../shaders/includes/ParticleShader.hpp"

class ParticleRenderer : public Renderer {
public:
  ParticleRenderer(ParticleShader *shader);
  virtual ~ParticleRenderer();

  virtual void onEnable() override;
  virtual void onDisable() override;

  ParticleShader *p_shader;
};
