#pragma once

#include "Shader.hpp"

class ShadowShader : public Shader {
public:
  ShadowShader();
  void init(const char *vertex_shader_path, const char* fragment_shader_path);

  GLint position_attr_loc;
  GLint pos_offset_attr_loc;

  GLint l_mvp_matrix_loc;
  GLint using_offset_loc;
};
