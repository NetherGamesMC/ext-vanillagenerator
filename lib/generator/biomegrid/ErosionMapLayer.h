#ifndef EXT_VANILLAGENERATOR_LIB_GENERATOR_BIOMEGRID_EROSIONMAPLAYER_H_
#define EXT_VANILLAGENERATOR_LIB_GENERATOR_BIOMEGRID_EROSIONMAPLAYER_H_

#include <utility>
#include "MapLayer.h"

using namespace GridBiome;

class ErosionMapLayer : public MapLayer {
 public:
  ErosionMapLayer(int_fast64_t seed, std::shared_ptr<MapLayer> belowLayer) : MapLayer(seed), belowLayer_(std::move(belowLayer)) {}

  ~ErosionMapLayer();

  BiomeGrid GenerateValues(int x, int z, int sizeX, int sizeZ) override;

 private:
  std::shared_ptr<MapLayer> belowLayer_;
};

#endif //EXT_VANILLAGENERATOR_LIB_GENERATOR_BIOMEGRID_EROSIONMAPLAYER_H_
