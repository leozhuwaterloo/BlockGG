#version 330

struct Material {
  vec3 kd;
  vec3 ks;
  float shininess;
};

in VsOutFsIn {
  vec2 uv_coord;
	vec3 shadow;
  vec3 light_dir;
  vec3 view_dir;
} fs_in;

out vec4 frag_color;

uniform Material material;
uniform vec3 ambient_intensity;
uniform vec3 light_rgb_intensity;

uniform sampler2D shadow_map;
uniform sampler2D normal_map;
uniform sampler2D m_texture;
uniform bool using_texture;

float getShadow(vec3 normal) {
  if(fs_in.shadow.z > 1.0) return 0.0;

  float shadow = 0.0;
  vec2 tex_size = 1.0 / textureSize(shadow_map, 0);
  float bias = 1.0;
  for(int x = -1; x <= 1; ++x) {
    for(int y = -1; y <= 1; ++y) {
      float pcf_depth = texture(shadow_map, fs_in.shadow.xy + vec2(x, y) * tex_size).r;
      shadow += (fs_in.shadow.z - bias) > pcf_depth ? 0.8 : 0.0;
    }
  }
  shadow /= 9.0;
  return shadow;
}

vec3 phongModel() {
  vec3 l = fs_in.light_dir;
  vec3 v = fs_in.view_dir;
  vec3 normal = texture(normal_map, fs_in.uv_coord).rgb;
  normal = normalize(normal * 2.0 - 1.0);
  float n_dot_l = max(dot(normal, l), 0.0);
  vec3 diffuse = material.kd * n_dot_l;
  vec3 specular = vec3(0.0);

  if (n_dot_l > 0.0) {
    vec3 h = normalize(v + l);
    float n_dot_h = max(dot(normal, h), 0.0);
    specular = material.ks * pow(n_dot_h, material.shininess);
  }
  float shadow = getShadow(normal);
  return ambient_intensity * material.kd + (1.0 - shadow) * light_rgb_intensity * (diffuse + specular);
}

void main() {
  frag_color = vec4(phongModel(), 1.0);
  if (using_texture) {
    vec4 tex_color = texture(m_texture, fs_in.uv_coord);
    if(tex_color.a < 0.05) discard;
    frag_color *= tex_color;
  }
}
