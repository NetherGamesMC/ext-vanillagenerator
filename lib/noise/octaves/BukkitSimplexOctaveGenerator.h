#ifndef EXT_VANILLAGENERATOR_LIB_VANILLA_BUKKITSIMPLEXOCTAVEGENERATOR_H_
#define EXT_VANILLAGENERATOR_LIB_VANILLA_BUKKITSIMPLEXOCTAVEGENERATOR_H_

#include <lib/objects/random/Random.h>
#include <vector>

#include "lib/noise/BukkitSimplexNoiseGenerator.h"

class BukkitSimplexOctaveGenerator {
 public:
  explicit BukkitSimplexOctaveGenerator(Random &random, int octaves);

  double Noise(double x, double y, double frequency, double amplitude, bool normalized);

 private:
  std::vector<BukkitSimplexNoiseGenerator> simplexOctaves;
};

#endif // EXT_VANILLAGENERATOR_LIB_VANILLA_BUKKITSIMPLEXOCTAVEGENERATOR_H_
