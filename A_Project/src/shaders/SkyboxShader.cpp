#include "./includes/SkyboxShader.hpp"

SkyboxShader::SkyboxShader():Shader() {}

void SkyboxShader::init(const char *vertex_shader_path,
  const char* fragment_shader_path) {
  generateProgramObject();
  attachVertexShader(vertex_shader_path);
  attachFragmentShader(fragment_shader_path);
  link();

  position_attr_loc = getAttribLocation("position");

  projection_loc = getUniformLocation("projection");
  view_loc = getUniformLocation("view");
}
