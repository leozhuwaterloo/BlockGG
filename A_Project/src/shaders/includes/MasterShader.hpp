#pragma once

#include "Shader.hpp"

class MasterShader : public Shader {
public:
  MasterShader();
  void init(const char *vertex_shader_path, const char* fragment_shader_path);

  GLint position_attr_loc;
  GLint normal_attr_loc;
  GLint uv_coord_attr_loc;
  GLint pos_offset_attr_loc;

  GLint model_loc;
  GLint view_loc;
  GLint projection_loc;
  GLint light_position_loc;
  GLint light_rgb_intensity_loc;
  GLint ambient_intensity_loc;
  GLint material_kd_loc;
  GLint material_ks_loc;
  GLint material_shininess_loc;
  GLint using_texture_loc;
  GLint l_vp_matrix_loc;
  GLint view_position_loc;
  GLint using_offset_loc;

  GLint m_texture_loc;
  GLint shadow_map_loc;
  GLint m_texture_index;
  GLint shadow_map_index;
};
