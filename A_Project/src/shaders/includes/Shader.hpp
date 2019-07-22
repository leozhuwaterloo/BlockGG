#pragma once

#include "cs488-framework/ShaderProgram.hpp"

class Shader : public ShaderProgram {
public:
  Shader();
  virtual void init(const char *vertex_shader_path, const char* fragment_shader_path) = 0;
};
