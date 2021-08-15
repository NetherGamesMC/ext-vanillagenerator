#ifndef EXT_VANILLAGENERATOR_LIB_GENERATOR_BIOMEGRID_SMOOTHMAPLAYER_H_
#define EXT_VANILLAGENERATOR_LIB_GENERATOR_BIOMEGRID_SMOOTHMAPLAYER_H_

#include <lib/objects/constants/BiomeList.h>

#include <utility>
#include "MapLayer.h"

using namespace GridBiome;

class SmoothMapLayer : public MapLayer {
 public:
  SmoothMapLayer(int_fast64_t seed, std::shared_ptr<MapLayer> belowLayer) : MapLayer(seed), belowLayer_(std::move(belowLayer)) {}

  ~SmoothMapLayer();

  BiomeGrid GenerateValues(int x, int z, int sizeX, int sizeZ) override;

 private:
  std::shared_ptr<MapLayer> belowLayer_;
};

#endif //EXT_VANILLAGENERATOR_LIB_GENERATOR_BIOMEGRID_SMOOTHMAPLAYER_H_
