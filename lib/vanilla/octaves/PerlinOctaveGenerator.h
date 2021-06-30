#ifndef EXT_NOISELIB_LIB_VANILLA_PERLINOCTAVEGENERATOR_H_
#define EXT_NOISELIB_LIB_VANILLA_PERLINOCTAVEGENERATOR_H_

#include <lib/random/Random.h>
#include <vector>

#include "ScalableOctaves.h"
#include "lib/vanilla/PerlinNoise.h"

class PerlinOctaveGenerator : public ScalableOctaves {
 public:
  explicit PerlinOctaveGenerator(Random &random, int octavesNum, int size_xv, int size_yv, int size_zv);

  std::vector<double> getFractalBrownianMotion(double x, double y, double z, double lacunarity, double persistence);

  int getArraySize() const;

  int getSizeY() const;

 private:
  std::vector<PerlinNoise> perlinOctaves;

  int sizeX;
  int sizeY;
  int sizeZ;
};

#endif //EXT_NOISELIB_LIB_VANILLA_PERLINOCTAVEGENERATOR_H_
