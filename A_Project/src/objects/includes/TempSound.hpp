#pragma once
#include "GameObject.hpp"

class TempSound : public GameObject {
public:
  TempSound(const glm::vec3 &pos, const ALuint &sound, const float &pitch, const float &gain);
  ~TempSound();
  virtual void tick() override;
  ALuint sound_source;
  float elapsed_time;
};
