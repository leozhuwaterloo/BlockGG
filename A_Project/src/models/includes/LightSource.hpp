#pragma once
#include <glm/glm.hpp>

struct LightSource {
	glm::vec3 pos;
	glm::vec3 rgb_intensity;
	glm::vec3 init_pos;
	LightSource(const glm::vec3 &pos, const glm::vec3 &rgb_intensity)
		: pos(pos), init_pos(pos), rgb_intensity(rgb_intensity) {}
};
