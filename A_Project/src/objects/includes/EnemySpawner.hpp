#pragma once
#include "GameObject.hpp"

class EnemySpawner : public GameObject {
public:
  EnemySpawner();
  virtual ~EnemySpawner();

  virtual void tick() override;
  float elapsed_time;
  float spawn_interval;
};
