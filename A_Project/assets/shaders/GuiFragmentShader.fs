#version 330

in VsOutFsIn {
  vec2 uv_coord;
} fs_in;

out vec4 frag_color;

uniform sampler2D m_texture;
uniform bool using_texture;
uniform vec4 color;

void main() {
  frag_color = color;
  if (using_texture) {
    vec4 tex_color = texture(m_texture, fs_in.uv_coord);
    if(tex_color.a < 0.05) discard;
    frag_color *= tex_color;
  }
}
