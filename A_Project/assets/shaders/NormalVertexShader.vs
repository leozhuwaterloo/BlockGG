#version 330

// Model-Space coordinates
in vec3 position;
in vec3 normal;
in vec2 uv_coord;
in vec3 tangent;
in vec3 pos_offset;

out VsOutFsIn {
	vec2 uv_coord;
	vec3 shadow;
	vec3 light_dir;
	vec3 view_dir;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform mat4 l_vp_matrix;
uniform bool using_offset;
uniform bool using_normal_map;
uniform vec3 light_position;
uniform vec3 view_position;

void main() {
	vec4 pos4 = vec4(position, 1.0);
	if (using_offset) pos4 += vec4(pos_offset, 0.0);

	pos4 = model * pos4;
	vs_out.light_dir = light_position - pos4.xyz;
	vs_out.view_dir = view_position - pos4.xyz;

	vec3 normal = normalize(vec3(model * vec4(normal, 0.0)));
	vec3 tangent = normalize(vec3(model * vec4(tangent, 0.0)));
	tangent = normalize(tangent - dot(tangent, normal) * normal);
	vec3 bitangent = cross(normal, tangent);
	mat3 to_tang = transpose(mat3(tangent, bitangent, normal));

	vs_out.light_dir = normalize(to_tang * vs_out.light_dir);
	vs_out.view_dir = normalize(to_tang * vs_out.view_dir);
	vs_out.uv_coord = uv_coord;
	vec4 l_pos4 = l_vp_matrix * pos4;
	vs_out.shadow = (l_pos4.xyz  / l_pos4.w) * 0.5 + 0.5;
  gl_Position = projection * view * pos4;
}
