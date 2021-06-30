#ifndef EXT_NOISELIB_LIB_VANILLA_BUKKITSIMPLEXOCTAVEGENERATOR_H_
#define EXT_NOISELIB_LIB_VANILLA_BUKKITSIMPLEXOCTAVEGENERATOR_H_

#include <lib/random/Random.h>
#include <vector>

#include "lib/vanilla/BukkitSimplexNoiseGenerator.h"

class BukkitSimplexOctaveGenerator {
 public:
  explicit BukkitSimplexOctaveGenerator(Random &random, int octaves);

  double noise(double x, double y, double frequency, double amplitude, bool normalized);

 private:
  std::vector<BukkitSimplexNoiseGenerator> simplexOctaves;
};

#endif // EXT_NOISELIB_LIB_VANILLA_BUKKITSIMPLEXOCTAVEGENERATOR_H_
