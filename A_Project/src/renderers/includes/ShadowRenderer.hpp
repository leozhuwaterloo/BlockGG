#pragma once
#include "Renderer.hpp"
#include "../../shaders/includes/ShadowShader.hpp"
#include "../../models/includes/GeometryNode.hpp"

class ShadowRenderer : public Renderer {
public:
  ShadowRenderer(ShadowShader *shader);
  virtual ~ShadowRenderer();

  virtual void onEnable() override;
  virtual void onDisable() override;

  virtual void updateShaderUniforms(const GeometryNode &node);

  ShadowShader *s_shader;
	GLuint fbo_depth_map;
  GLuint depth_map;
  unsigned int shadow_width, shadow_height;
};
