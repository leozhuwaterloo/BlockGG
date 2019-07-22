#pragma once
#include <string>
#include <vector>
#include <glm/glm.hpp>

namespace CommonUtils {
  std::string format(const char *fmt, ...);
  void getFiles(const char *dir, std::vector<std::string> &ret_vector);
  glm::vec3 getRandomVec3();
  float randFloat();
}
