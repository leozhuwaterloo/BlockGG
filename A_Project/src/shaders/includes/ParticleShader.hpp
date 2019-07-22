#pragma once

#include "Shader.hpp"

class ParticleShader : public Shader {
public:
  ParticleShader();
  void init(const char *vertex_shader_path, const char* fragment_shader_path);

  GLint position_attr_loc;
  GLint model_view_attr_loc;

  GLint projection_loc;
  GLint color_loc;
  GLint using_texture_loc;
};
