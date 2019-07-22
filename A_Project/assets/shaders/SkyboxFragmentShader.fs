#version 330

in VsOutFsIn {
	vec3 texture_dir;
} fs_in;

out vec4 frag_color;
uniform samplerCube cube_map;

void main() {
  frag_color = texture(cube_map, fs_in.texture_dir);
}
