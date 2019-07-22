#pragma once
#include <vector>

class PerlinNoise {
public:
  PerlinNoise(const unsigned int &seed);
  PerlinNoise();
  virtual ~PerlinNoise();

  double noise(const double &x, const double &y, const double &z, const double &freq);
private:
  std::vector<int> p;
  double grad(const int &hash, const double &x, const double &y, const double &z);
  double fade(const double &t);
};
