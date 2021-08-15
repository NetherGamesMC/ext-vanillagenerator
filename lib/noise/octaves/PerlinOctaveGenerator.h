#ifndef EXT_VANILLAGENERATOR_LIB_VANILLA_PERLINOCTAVEGENERATOR_H_
#define EXT_VANILLAGENERATOR_LIB_VANILLA_PERLINOCTAVEGENERATOR_H_

#include <lib/objects/random/Random.h>
#include <vector>

#include "ScalableOctaves.h"
#include "lib/noise/PerlinNoise.h"

class PerlinOctaveGenerator : public ScalableOctaves {
 public:
  explicit PerlinOctaveGenerator(Random &random, int octavesNum, int sizeXv, int sizeYv, int sizeZv);

  std::vector<double> GetFractalBrownianMotion(double x, double y, double z, double lacunarity, double persistence);

  int GetArraySize() const;

  int GetSizeY() const;

 private:
  std::vector<PerlinNoise> perlinOctaves;

  int sizeX;
  int sizeY;
  int sizeZ;
};

#endif //EXT_VANILLAGENERATOR_LIB_VANILLA_PERLINOCTAVEGENERATOR_H_
