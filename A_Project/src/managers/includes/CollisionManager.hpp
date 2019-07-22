#pragma once
#include "ResourceManager.hpp"

class CollisionManager {
public:
  CollisionManager();
  virtual ~CollisionManager();
  void tick();
  ResourceManager *resource_manager;
};
