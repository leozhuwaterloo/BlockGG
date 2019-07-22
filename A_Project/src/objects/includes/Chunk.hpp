#pragma once
#include "MeshObjects.hpp"
class HeightGetter;

class Chunk : public MeshGroups {
public:
  Chunk(const glm::vec3 &pos, const float &size, HeightGetter *height_getter);
  virtual ~Chunk();
protected:
  float size;
  HeightGetter *height_getter;
};
