#ifndef EXT_NOISELIB_LIB_GENERATOR_BIOMEGRID_EROSIONMAPLAYER_H_
#define EXT_NOISELIB_LIB_GENERATOR_BIOMEGRID_EROSIONMAPLAYER_H_

#include <utility>
#include "MapLayer.h"

using namespace GridBiome;

class ErosionMapLayer : public MapLayer {
 public:
  ErosionMapLayer(int_fast32_t seed, std::shared_ptr<MapLayer> belowLayer) : MapLayer(seed), below_layer_(std::move(belowLayer)) {}

  ~ErosionMapLayer();

  BiomeGrid GenerateValues(int x, int z, int size_x, int size_z) override;

 private:
  std::shared_ptr<MapLayer> below_layer_;
};

#endif //EXT_NOISELIB_LIB_GENERATOR_BIOMEGRID_EROSIONMAPLAYER_H_
