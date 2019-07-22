#include "./includes/CommonUtils.hpp"
#include "cs488-framework/Exception.hpp"
#include <stdarg.h>
#include <dirent.h>

std::string CommonUtils::format(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  int length = vsnprintf(nullptr, 0, fmt, args) + 1;
  va_end(args);
  va_start(args, fmt);
  char buffer[length];
  vsnprintf(buffer, length, fmt, args);
  va_end(args);
  return std::string(buffer);
}

float CommonUtils::randFloat() {
  return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
}

glm::vec3 CommonUtils::getRandomVec3() {
  glm::vec3 r;
  for(int i = 0; i < 3; ++i) {
    r[i] = randFloat() - 0.5f;
  }
  return r;
}

void CommonUtils::getFiles(const char *dir,
  std::vector<std::string> &ret_vector)
{
  DIR *d;
  struct dirent *ent;
  if ((d = opendir(dir)) != nullptr) {
    while ((ent = readdir(d)) != nullptr) {
      std::string name = std::string(ent->d_name);
      if (name == "." || name == ".." || name == "unused") continue;
      ret_vector.push_back(name);
    }
    closedir(d);
  } else {
    throw Exception("Could not open " + std::string(dir));
  }
}
