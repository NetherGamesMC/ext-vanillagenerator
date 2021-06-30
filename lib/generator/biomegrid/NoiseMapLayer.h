#ifndef EXT_NOISELIB_LIB_GENERATOR_BIOMEGRID_NOISEMAPLAYER_H_
#define EXT_NOISELIB_LIB_GENERATOR_BIOMEGRID_NOISEMAPLAYER_H_

#include "MapLayer.h"

#include <lib/vanilla/octaves/BukkitSimplexOctaveGenerator.h>
#include <utility>
#include <array>

using namespace GridBiome;

class NoiseMapLayer : public MapLayer {
 public:
  explicit NoiseMapLayer(int_fast64_t seed) : MapLayer(seed) {
    noise_gen_seed_ = new Random(seed);
    noise_gen_ = new BukkitSimplexOctaveGenerator(*noise_gen_seed_, 2);
  }

  BlockValues GenerateValues(int x, int z, int size_x, int size_z) override;
 private:
  Random *noise_gen_seed_;
  BukkitSimplexOctaveGenerator *noise_gen_;
};

#endif //EXT_NOISELIB_LIB_GENERATOR_BIOMEGRID_NOISEMAPLAYER_H_
