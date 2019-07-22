#pragma once
#include "Renderer.hpp"
#include "../../shaders/includes/MasterShader.hpp"
#include "../../models/includes/GeometryNode.hpp"

class MasterRenderer : public Renderer {
public:
  MasterRenderer(MasterShader *shader);
  virtual ~MasterRenderer();

  virtual void onEnable() override;
  virtual void onDisable() override;

  virtual void updateGlobalShaderUniforms();
  virtual void updateShaderUniforms(const GeometryNode &node);
  MasterShader *m_shader;
};
