#pragma once
#include "Renderer.hpp"
#include "../../shaders/includes/NormalShader.hpp"
#include "../../models/includes/GeometryNode.hpp"

class NormalRenderer : public Renderer {
public:
  NormalRenderer(NormalShader *shader);
  virtual ~NormalRenderer();

  virtual void onEnable() override;
  virtual void onDisable() override;

  virtual void updateGlobalShaderUniforms();
  virtual void updateShaderUniforms(const GeometryNode &node);
  NormalShader *n_shader;
};
