#ifndef EXT_VANILLAGENERATOR_LIB_GENERATOR_BIOMEGRID_NOISEMAPLAYER_H_
#define EXT_VANILLAGENERATOR_LIB_GENERATOR_BIOMEGRID_NOISEMAPLAYER_H_

#include "MapLayer.h"

#include <lib/noise/octaves/BukkitSimplexOctaveGenerator.h>
#include <utility>
#include <array>

using namespace GridBiome;

class NoiseMapLayer : public MapLayer {
 public:
  explicit NoiseMapLayer(int_fast64_t seed) : MapLayer(seed), noiseGenSeed_(seed), noiseGen_(noiseGenSeed_, 2) {}

  BiomeGrid GenerateValues(int x, int z, int sizeX, int sizeZ) override;
 private:
  Random noiseGenSeed_;
  BukkitSimplexOctaveGenerator noiseGen_;
};

#endif //EXT_VANILLAGENERATOR_LIB_GENERATOR_BIOMEGRID_NOISEMAPLAYER_H_
