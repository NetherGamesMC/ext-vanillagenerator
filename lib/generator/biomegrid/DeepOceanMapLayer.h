#ifndef EXT_NOISELIB_LIB_GENERATOR_BIOMEGRID_DEEPOCEANMAPLAYER_H_
#define EXT_NOISELIB_LIB_GENERATOR_BIOMEGRID_DEEPOCEANMAPLAYER_H_

#include "MapLayer.h"

using namespace GridBiome;

class DeepOceanMapLayer : public MapLayer {
 public:
  DeepOceanMapLayer(int_fast64_t seed, MapLayer *belowLayer) : MapLayer(seed), below_layer_(belowLayer) {}

  BlockValues GenerateValues(int x, int z, int size_x, int size_z) override;

 private:
  MapLayer *below_layer_;
};

#endif //EXT_NOISELIB_LIB_GENERATOR_BIOMEGRID_DEEPOCEANMAPLAYER_H_
