#version 330

// Model-Space coordinates
in vec3 position;
in vec3 pos_offset;

uniform mat4 l_mvp_matrix;
uniform bool using_offset;

void main() {
	vec4 pos4 = vec4(position, 1.0);
  if (using_offset) pos4 += vec4(pos_offset, 0.0);

  gl_Position = l_mvp_matrix * pos4;
}
