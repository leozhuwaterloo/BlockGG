#pragma once
#include <glm/glm.hpp>
#include "../../managers/includes/ResourceManager.hpp"


enum class ObjectType {
	GroundEarthBlock,
	GroundSnowBlock,
	Stone4Block,
	Stone6Block,
	Crate,
	Player,
	Chunk,
	Bullet,
	Terrain,
	Tree1,
	Grass,
	ColoredParticleEmitter,
	ColoredParticle,
	GunSMG,
	EnemySpawner,
	Slime,
	TempSound,
	Skybox
};
static std::string OBJECT_TYPE_STR[] = {
	"GroundEarthBlock",
	"GroundSnowBlock",
	"Stone4Block",
	"Stone6Block",
	"Crate",
	"Player",
	"Chunk",
	"Bullet",
	"Terrain",
	"Tree1",
	"Grass",
	"ColoredParticleEmitter",
	"ColoredParticle",
	"GunSMG",
	"EnemySpawner",
	"Slime",
	"TempSound",
	"Skybox"
};

class GameObject {
public:
	GameObject(const ObjectType &type, const glm::vec3 &pos);
  GameObject(const ObjectType &type, const glm::vec3 &pos, const glm::vec3 &rot);
  virtual ~GameObject() = 0;

	unsigned int id;
	ObjectType type;
	glm::vec3 pos, rot;
  virtual void tick() = 0;
	virtual void checkCollideTerrain(const glm::vec3 &bound, glm::vec3 &dis);
	GameObject *next;
	GameObject *parent;
protected:
  ResourceManager *resource_manager;
};
