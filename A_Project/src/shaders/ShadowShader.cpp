#include "./includes/ShadowShader.hpp"

ShadowShader::ShadowShader():Shader() {}

void ShadowShader::init(const char *vertex_shader_path,
  const char* fragment_shader_path) {
  generateProgramObject();
  attachVertexShader(vertex_shader_path);
  attachFragmentShader(fragment_shader_path);
  link();

  position_attr_loc = getAttribLocation("position");
  pos_offset_attr_loc = getAttribLocation("pos_offset");

  using_offset_loc = getUniformLocation("using_offset");
  l_mvp_matrix_loc = getUniformLocation("l_mvp_matrix");
}
