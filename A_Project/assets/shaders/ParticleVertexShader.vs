#version 330

in vec2 position;
in mat4 model_view;

out VsOutFsIn {
	vec2 uv_coord;
} vs_out;

uniform mat4 projection;

void main() {
	vs_out.uv_coord = vec2((position.x + 1.0) / 2.0, 1 - (position.y + 1.0) / 2.0);
	vec4 pos4 = vec4(position, 0.0, 1.0);
  gl_Position = projection * model_view * pos4;
}
