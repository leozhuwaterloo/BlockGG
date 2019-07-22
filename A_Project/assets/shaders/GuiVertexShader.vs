#version 330

in vec2 position;

out VsOutFsIn {
	vec2 uv_coord;
} vs_out;

uniform mat4 transformation;
uniform bool using_texture;

void main() {
	if (using_texture) {
		vs_out.uv_coord = vec2((position.x + 1.0) / 2.0, 1 - (position.y + 1.0) / 2.0);
	}
  gl_Position = transformation * vec4(position, 0.0, 1.0);
}
