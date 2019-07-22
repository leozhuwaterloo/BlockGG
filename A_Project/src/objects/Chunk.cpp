#include "./includes/Chunk.hpp"
#include "./includes/Blocks.hpp"
#include "./includes/Terrain.hpp"
#include "./includes/Enviroments.hpp"
#include "cs488-framework/GlErrorCheck.hpp"
#include "../utils/includes/CommonUtils.hpp"

static int snow_height = 15;
static int snow_fade_distance = 15;
static int stone_height = -18;
static int stone_fade_distance = 15;
static void add(const float &y, const glm::vec3 &pos,
  std::vector<glm::vec3> &earth, std::vector<glm::vec3> &snow,
  std::vector<glm::vec3> &stone, std::vector<glm::vec3> &tree1,
  std::vector<glm::vec3> &grass)
{
  bool earth_pushed(false);
  if (y > snow_height) {
    snow.push_back(pos);
  } else if (y > snow_height - snow_fade_distance) {
    if (CommonUtils::randFloat() < (y - snow_height + snow_fade_distance) / snow_fade_distance) {
      snow.push_back(pos);
    } else {
      earth.push_back(pos);
      earth_pushed = true;
    }
  } else if (y < stone_height){
    stone.push_back(pos);
  } else if (y < stone_height + stone_fade_distance) {
    if (CommonUtils::randFloat() < (stone_height + stone_fade_distance - y) / stone_fade_distance) {
      stone.push_back(pos);
    } else {
      earth.push_back(pos);
      earth_pushed = true;
    }
  } else {
    earth.push_back(pos);
    earth_pushed = true;
  }

  if (CommonUtils::randFloat() > 0.999) {
    tree1.push_back(pos * 0.5f);
  }

  if (CommonUtils::randFloat() > 0.99) {
    grass.push_back(pos);
  }
}

Chunk::Chunk(const glm::vec3 &pos, const float &size,
    HeightGetter *height_getter)
  : MeshGroups(ObjectType::Chunk, pos),
    size(size),
    height_getter(height_getter)
{
  std::vector<glm::vec3> earth;
  std::vector<glm::vec3> snow;
  std::vector<glm::vec3> stone;
  std::vector<glm::vec3> tree1;
  std::vector<glm::vec3> dump;
  std::vector<glm::vec3> grass;

  for(int x = -size + pos.x; x <= size + pos.x; ++x) {
    for(int z = -size + pos.z; z <= size + pos.z; ++z) {
      float y = pos.y + height_getter->getHeight(x, z);
      add(y, glm::vec3(x, y, z), earth, snow, stone, tree1, grass);
    }
  }
  // filter blank spots
  for(int x = -size + pos.x; x <= size + pos.x; ++x) {
    int pre_y = pos.y + height_getter->getHeight(x, -size + pos.z);
    for(int z = -size + pos.z + 1; z <= size + pos.z + 1; ++z) {
      int y = pos.y + height_getter->getHeight(x, z);
      while (glm::abs(pre_y - y) > 1) {
        if (pre_y > y) {
          add(pre_y - 1, glm::vec3(x, (--pre_y), (z - 1)), earth, snow, stone, dump, dump);
        } else {
          add(y - 1, glm::vec3(x, (--y), z), earth, snow, stone, dump, dump);
        }
      }
      pre_y = y;
    }
  }

  for(int z = -size + pos.z; z <= size + pos.z; ++z) {
    int pre_y = pos.y + height_getter->getHeight(-size + pos.x, z);
    for(int x = -size + pos.x + 1; x <= size + pos.x + 1; ++x) {
      int y = pos.y + height_getter->getHeight(x, z);
      while (glm::abs(pre_y - y) > 1) {
        if (pre_y > y) {
          add(pre_y - 1, glm::vec3((x - 1), (--pre_y), z), earth, snow, stone, dump, dump);
        } else {
          add(y - 1, glm::vec3(x, (--y), z), earth, snow, stone, dump, dump);
        }
      }
      pre_y = y;
    }
  }

  instance_list.push_back(std::move(std::unique_ptr<GroundEarthBlock>(
    new GroundEarthBlock(glm::vec3(pos)))));
  instance_list.push_back(std::move(std::unique_ptr<GroundSnowBlock>(
    new GroundSnowBlock(glm::vec3(pos)))));
  instance_list.push_back(std::move(std::unique_ptr<Stone4Block>(
    new Stone4Block(glm::vec3(pos)))));
  instance_list.push_back(std::move(std::unique_ptr<Tree1>(
    new Tree1(glm::vec3(pos)))));
  instance_list.push_back(std::move(std::unique_ptr<Grass>(
    new Grass(glm::vec3(pos)))));


  data_count_list.push_back(earth.size());
  data_count_list.push_back(snow.size());
  data_count_list.push_back(stone.size());
  data_count_list.push_back(tree1.size());
  data_count_list.push_back(grass.size());

  pos_offset_list.resize(5);
  glGenBuffers(1, &pos_offset_list[0]);
  glBindBuffer(GL_ARRAY_BUFFER, pos_offset_list[0]);
  glBufferData(GL_ARRAY_BUFFER, earth.size() * sizeof(glm::vec3),
    &(earth[0].x), GL_STATIC_DRAW);

  glGenBuffers(1, &pos_offset_list[1]);
  glBindBuffer(GL_ARRAY_BUFFER, pos_offset_list[1]);
  glBufferData(GL_ARRAY_BUFFER, snow.size() * sizeof(glm::vec3),
    &(snow[0].x), GL_STATIC_DRAW);

  glGenBuffers(1, &pos_offset_list[2]);
  glBindBuffer(GL_ARRAY_BUFFER, pos_offset_list[2]);
  glBufferData(GL_ARRAY_BUFFER, stone.size() * sizeof(glm::vec3),
    &(stone[0].x), GL_STATIC_DRAW);

  glGenBuffers(1, &pos_offset_list[3]);
  glBindBuffer(GL_ARRAY_BUFFER, pos_offset_list[3]);
  glBufferData(GL_ARRAY_BUFFER, tree1.size() * sizeof(glm::vec3),
    &(tree1[0].x), GL_STATIC_DRAW);
  CHECK_GL_ERRORS;

  glGenBuffers(1, &pos_offset_list[4]);
  glBindBuffer(GL_ARRAY_BUFFER, pos_offset_list[4]);
  glBufferData(GL_ARRAY_BUFFER, grass.size() * sizeof(glm::vec3),
    &(grass[0].x), GL_STATIC_DRAW);
  CHECK_GL_ERRORS;
}
Chunk::~Chunk() {
  for(int i = 0; i < pos_offset_list.size(); ++i) {
    glDeleteBuffers(1, &pos_offset_list[i]);
  }
}
