#pragma once
#include <glm/glm.hpp>

class Bound {
public:
  glm::vec3 min;
  glm::vec3 max;
  Bound(const glm::vec3 &min, const glm::vec3 &max);
};

class Collidable {
public:
  virtual Bound getBound() = 0;
  virtual bool collide(Collidable *other);
};
