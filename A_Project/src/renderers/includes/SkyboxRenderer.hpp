#pragma once
#include "Renderer.hpp"
#include "../../shaders/includes/SkyboxShader.hpp"
#include "../../models/includes/GeometryNode.hpp"

class SkyboxRenderer : public Renderer {
public:
  SkyboxRenderer(SkyboxShader *shader);
  virtual ~SkyboxRenderer();

  virtual void onEnable() override;
  virtual void onDisable() override;

  SkyboxShader *b_shader;
};
