#include "./includes/PerlinNoise.hpp"
#include <random>
#include <algorithm>
#include <glm/glm.hpp>
// https://cs.nyu.edu/~perlin/noise/

PerlinNoise::PerlinNoise(const unsigned int &seed) {
  int indx = 0;
  p.resize(256);
  std::generate(p.begin(), p.end(), [&indx] {return indx++;});
  std::default_random_engine engine(seed);
  std::shuffle(p.begin(), p.end(), engine);
  p.insert(p.end(), p.begin(), p.end());
}

PerlinNoise::PerlinNoise(): PerlinNoise(0) {}

PerlinNoise::~PerlinNoise() {}

double PerlinNoise::noise(const double &i_x, const double &i_y, const double &i_z, const double &freq) {
  double x = i_x / freq, y = i_y / freq, z = i_z / freq;
  int X = (int)glm::floor(x) & 255,
    Y = (int)glm::floor(y) & 255,
    Z = (int)glm::floor(z) & 255;

  x -= glm::floor(x),
  y -= glm::floor(y),
  z -= glm::floor(z);
  double u = fade(x), v = fade(y), w = fade(z);

  int A = p[X] + Y, AA = p[A] + Z, AB = p[A + 1] + Z;
  int B = p[X + 1] + Y, BA = p[B] + Z, BB = p[B + 1] + Z;

  return glm::mix(glm::mix(glm::mix(grad(p[AA], x, y, z), grad(p[BA], x-1, y, z), u),
    glm::mix(grad(p[AB], x, y-1, z), grad(p[BB], x-1, y-1, z), u), v),
    glm::mix(glm::mix(grad(p[AA+1], x, y, z-1), grad(p[BA+1], x-1, y, z-1), u),
      glm::mix(grad(p[AB+1], x, y-1, z-1), grad(p[BB+1], x-1, y-1, z-1), u), v), w);
}

double PerlinNoise::fade(const double &t) {
  return t * t * t * (t * (t * 6 - 15) + 10);
}

double PerlinNoise::grad(const int &hash, const double &x, const double &y, const double &z) {
  int h = hash & 15;                    // CONVERT LO 4 BITS OF HASH CODE
  double u = h < 8 ? x : y,             // INTO 12 GRADIENT DIRECTIONS.
    v = h < 4 ? y : h == 12 || h == 14 ? x : z;
  return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}
