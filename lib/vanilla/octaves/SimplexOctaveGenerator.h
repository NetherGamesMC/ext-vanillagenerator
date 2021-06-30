#ifndef EXT_NOISELIB_LIB_VANILLA_SIMPLEXOCTAVEGENERATOR_H_
#define EXT_NOISELIB_LIB_VANILLA_SIMPLEXOCTAVEGENERATOR_H_

#include <lib/random/Random.h>
#include <vector>

#include "ScalableOctaves.h"
#include "lib/vanilla/SimplexNoise.h"

class SimplexOctaveGenerator : public ScalableOctaves {
 public:
  explicit SimplexOctaveGenerator(Random &random, int octavesNum, int size_xv, int size_yv, int size_zv);

  std::vector<double> getFractalBrownianMotion(double x, double y, double z, double lacunarity, double persistence);

  int getSizeX() const;

  int getSizeZ() const;

  int getArraySize() const;

  double noise(double x, double y, double z, double frequency, double amplitude, bool normalized = false);

 private:
  std::vector<SimplexNoise> simplexOctaves;

  int sizeX;
  int sizeY;
  int sizeZ;
};


#endif //EXT_NOISELIB_LIB_VANILLA_SIMPLEXOCTAVEGENERATOR_H_
