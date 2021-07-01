#ifndef EXT_NOISELIB_LIB_GENERATOR_BIOMEGRID_SMOOTHMAPLAYER_H_
#define EXT_NOISELIB_LIB_GENERATOR_BIOMEGRID_SMOOTHMAPLAYER_H_

#include <lib/pocketmine/BiomeList.h>
#include "MapLayer.h"

using namespace GridBiome;

class SmoothMapLayer : public MapLayer {
 public:
  SmoothMapLayer(int_fast64_t seed, MapLayer *belowLayer) : MapLayer(seed), below_layer_(belowLayer) {}

  BlockValues GenerateValues(int x, int z, int size_x, int size_z) override;

 private:
  MapLayer *below_layer_;
};

#endif //EXT_NOISELIB_LIB_GENERATOR_BIOMEGRID_SMOOTHMAPLAYER_H_
