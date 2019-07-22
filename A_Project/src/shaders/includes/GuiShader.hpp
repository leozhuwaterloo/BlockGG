#pragma once

#include "Shader.hpp"

class GuiShader : public Shader {
public:
  GuiShader();
  void init(const char *vertex_shader_path, const char* fragment_shader_path);

  GLint position_attr_loc;

  GLint transformation_loc;
  GLint using_texture_loc;
  GLint color_loc;
};
