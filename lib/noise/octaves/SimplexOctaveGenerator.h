#ifndef EXT_VANILLAGENERATOR_LIB_VANILLA_SIMPLEXOCTAVEGENERATOR_H_
#define EXT_VANILLAGENERATOR_LIB_VANILLA_SIMPLEXOCTAVEGENERATOR_H_

#include <vector>
#include <lib/objects/random/Random.h>

#include "ScalableOctaves.h"
#include "lib/noise/SimplexNoise.h"

class SimplexOctaveGenerator : public ScalableOctaves {
 public:
  SimplexOctaveGenerator(Random &random, int octavesNum, int sizeXv, int sizeYv, int sizeZv);

  std::vector<double> GetFractalBrownianMotion(double x, double y, double z, double lacunarity, double persistence);

  int GetSizeX() const;

  int GetSizeZ() const;

  int GetArraySize() const;

  double Noise(double x, double y, double z, double frequency, double amplitude, bool normalized = false);

 private:
  std::vector<SimplexNoise> simplexOctaves;

  int sizeX;
  int sizeY;
  int sizeZ;
};


#endif //EXT_VANILLAGENERATOR_LIB_VANILLA_SIMPLEXOCTAVEGENERATOR_H_
