#version 330

in vec3 position;

out VsOutFsIn {
	vec3 texture_dir;
} vs_out;

uniform mat4 projection;
uniform mat4 view;

void main() {
  gl_Position = projection * view * vec4(position, 1.0);
  vs_out.texture_dir = position;
}
