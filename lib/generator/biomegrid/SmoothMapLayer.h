#ifndef EXT_NOISELIB_LIB_GENERATOR_BIOMEGRID_SMOOTHMAPLAYER_H_
#define EXT_NOISELIB_LIB_GENERATOR_BIOMEGRID_SMOOTHMAPLAYER_H_

#include <lib/objects/constants/BiomeList.h>

#include <utility>
#include "MapLayer.h"

using namespace GridBiome;

class SmoothMapLayer : public MapLayer {
 public:
  SmoothMapLayer(int_fast64_t seed, std::shared_ptr<MapLayer> belowLayer) : MapLayer(seed), below_layer_(std::move(belowLayer)) {}

  ~SmoothMapLayer();

  BiomeGrid GenerateValues(int x, int z, int size_x, int size_z) override;

 private:
  std::shared_ptr<MapLayer> below_layer_;
};

#endif //EXT_NOISELIB_LIB_GENERATOR_BIOMEGRID_SMOOTHMAPLAYER_H_
