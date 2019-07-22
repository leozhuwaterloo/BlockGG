#pragma once

#include "Shader.hpp"

class SkyboxShader : public Shader {
public:
  SkyboxShader();
  void init(const char *vertex_shader_path, const char* fragment_shader_path);

  GLint position_attr_loc;

  GLint projection_loc;
  GLint view_loc;
};
