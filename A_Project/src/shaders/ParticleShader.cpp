#include "./includes/ParticleShader.hpp"


ParticleShader::ParticleShader():Shader() {}


void ParticleShader::init(const char *vertex_shader_path,
  const char* fragment_shader_path) {
  generateProgramObject();
  attachVertexShader(vertex_shader_path);
  attachFragmentShader(fragment_shader_path);
  link();

  position_attr_loc = getAttribLocation("position");
  model_view_attr_loc = getAttribLocation("model_view");

  projection_loc = getUniformLocation("projection");
  color_loc = getUniformLocation("color");
  using_texture_loc = getUniformLocation("using_texture");
}
