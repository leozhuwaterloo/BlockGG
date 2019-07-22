#pragma once
#include "Chunk.hpp"
#include "../../utils/includes/PerlinNoise.hpp"
#include "../../utils/includes/LinkedList.hpp"
#include <unordered_map>

class HeightGetter {
public:
  virtual float getHeight(const float &x, const float &z) = 0;
};

class Terrain : public GameObject, public HeightGetter {
public:
  Terrain(const glm::vec3 &pos);
  Terrain(const glm::vec3 &pos, const unsigned int &seed);
  ~Terrain();
  virtual float getHeight(const float &x, const float &z) override;
  virtual void tick() override;
  glm::vec2 getChunk(const glm::vec3 &pos);
  virtual void setRenderDistance(const float &render_distance);
protected:
  LinkedList<GameObject> chunks;
  std::unique_ptr<PerlinNoise> pn;
  float chunk_size;
  float chunk_dim;
  float render_distance, chunk_count;
  std::unordered_map<int, std::unordered_map<int, Chunk*>> rendered;
};
