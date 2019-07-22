#pragma once
#include "Renderer.hpp"
#include "../../shaders/includes/GuiShader.hpp"

class GuiRenderer : public Renderer {
public:
  GuiRenderer(GuiShader *shader);
  virtual ~GuiRenderer();

  virtual void onEnable() override;
  virtual void onDisable() override;

  GuiShader *g_shader;
};
