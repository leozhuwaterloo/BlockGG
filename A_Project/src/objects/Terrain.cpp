#include "./includes/Terrain.hpp"
#include "../managers/includes/GameManager.hpp"
#include "./includes/Enviroments.hpp"

Terrain::Terrain(const glm::vec3 &pos): Terrain(pos, 1) {}
Terrain::Terrain(const glm::vec3 &pos, const unsigned int &seed)
  : GameObject(ObjectType::Terrain, pos),
    pn(std::unique_ptr<PerlinNoise>(new PerlinNoise(seed))),
    chunk_size(20.0f),
    chunk_dim(2 * chunk_size + 1)
{
  setRenderDistance(150.0f);
}
Terrain::~Terrain() {}

float Terrain::getHeight(const float &f_x, const float &f_z) {
  int x = (int)glm::round(f_x);
  int z = (int)glm::round(f_z);
  return glm::floor(pn->noise(x, 1, z, 40) * 40);
}

void Terrain::tick() {
  // Render new chunks
  glm::vec3 player_pos = resource_manager->game->player->pos;
  glm::vec2 chunk_pos = getChunk(player_pos);
  std::unordered_map<int, std::unordered_map<int, bool>> current_render;
  for (int x = -chunk_count + chunk_pos.x; x <= chunk_count + chunk_pos.x; ++x) {
    for (int z = -chunk_count + chunk_pos.y; z <= chunk_count + chunk_pos.y; ++z) {
      if (rendered[x].find(z) == rendered[x].end() || rendered[x].at(z) == nullptr) {
        Chunk *new_chunk = new Chunk(glm::vec3(pos.x + x * chunk_dim,
            pos.y, pos.z + z * chunk_dim), chunk_size, this);
        rendered[x][z] = new_chunk;
        resource_manager->game->add(new_chunk);
      }
      current_render[x][z] = true;
    }
  }
  // Remove old chunks
  for (const auto &it : rendered) {
    int x = it.first;
    for (const auto &it2 : rendered.at(x)) {
      Chunk *obj = it2.second;
      if (obj != nullptr) {
        int z = it2.first;
        if (current_render[x].find(z) == current_render[x].end()) {
          resource_manager->game->remove(rendered[x][z]);
          rendered[x][z] = nullptr;
        }
      }
    }
  }
}

void Terrain::setRenderDistance(const float &render_distance) {
  this->render_distance = render_distance;
  chunk_count = glm::ceil((2 * render_distance + 1) / chunk_dim);
  chunk_count = glm::floor(chunk_count / 2);
}

glm::vec2 Terrain::getChunk(const glm::vec3 &pos) {
  return glm::vec2(glm::round(pos.x / chunk_dim), glm::round(pos.z / chunk_dim));
}
