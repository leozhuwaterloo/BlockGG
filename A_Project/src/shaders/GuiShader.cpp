#include "./includes/GuiShader.hpp"


GuiShader::GuiShader():Shader() {}


void GuiShader::init(const char *vertex_shader_path,
  const char* fragment_shader_path) {
  generateProgramObject();
  attachVertexShader(vertex_shader_path);
  attachFragmentShader(fragment_shader_path);
  link();

  position_attr_loc = getAttribLocation("position");

  transformation_loc = getUniformLocation("transformation");
  using_texture_loc = getUniformLocation("using_texture");
  color_loc = getUniformLocation("color");
}
