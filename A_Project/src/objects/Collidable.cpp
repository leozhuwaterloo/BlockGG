#include "./includes/Collidable.hpp"

Bound::Bound(const glm::vec3 &min, const glm::vec3 &max)
  : min(min), max(max) {}

bool Collidable::collide(Collidable *other) {
  Bound b1 = getBound();
  Bound b2 = other->getBound();
  return (b1.min.x <= b2.max.x && b2.min.x <= b1.max.x) &&
    (b1.min.y <= b2.max.y && b2.min.y <= b1.max.y) &&
    (b1.min.z <= b2.max.z && b2.min.z <= b1.max.z);
}
