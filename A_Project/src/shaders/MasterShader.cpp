#include "./includes/MasterShader.hpp"
#include "../managers/includes/ResourceManager.hpp"

MasterShader::MasterShader():Shader() {}


void MasterShader::init(const char *vertex_shader_path,
    const char* fragment_shader_path) {
  generateProgramObject();
  attachVertexShader(vertex_shader_path);
  attachFragmentShader(fragment_shader_path);
  link();

  position_attr_loc = getAttribLocation("position");
  normal_attr_loc = getAttribLocation("normal");
  uv_coord_attr_loc = getAttribLocation("uv_coord");
  pos_offset_attr_loc = getAttribLocation("pos_offset");

  model_loc = getUniformLocation("model");
  view_loc = getUniformLocation("view");
  projection_loc = getUniformLocation("projection");
  light_position_loc = getUniformLocation("light_position");
  light_rgb_intensity_loc = getUniformLocation("light_rgb_intensity");
  ambient_intensity_loc = getUniformLocation("ambient_intensity");
  material_kd_loc = getUniformLocation("material.kd");
  material_ks_loc = getUniformLocation("material.ks");
  material_shininess_loc = getUniformLocation("material.shininess");

  using_offset_loc = getUniformLocation("using_offset");
  using_texture_loc = getUniformLocation("using_texture");
  l_vp_matrix_loc = getUniformLocation("l_vp_matrix");
  view_position_loc = getUniformLocation("view_position");

  m_texture_loc = getUniformLocation("m_texture");
  shadow_map_loc = getUniformLocation("shadow_map");

  m_texture_index = 0;
  shadow_map_index = 1;
}
