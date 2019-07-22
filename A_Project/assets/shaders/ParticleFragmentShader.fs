#version 330

in VsOutFsIn {
  vec2 uv_coord;
} fs_in;

out vec4 frag_color;

uniform vec4 color;
uniform sampler2D m_texture;
uniform bool using_texture;

void main() {
  if (using_texture) {
    frag_color = texture(m_texture, fs_in.uv_coord);
  } else {
    frag_color = color;
  }
}
