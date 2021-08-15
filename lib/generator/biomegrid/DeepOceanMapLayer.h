#ifndef EXT_VANILLAGENERATOR_LIB_GENERATOR_BIOMEGRID_DEEPOCEANMAPLAYER_H_
#define EXT_VANILLAGENERATOR_LIB_GENERATOR_BIOMEGRID_DEEPOCEANMAPLAYER_H_


#include <utility>
#include "MapLayer.h"

using namespace GridBiome;

class DeepOceanMapLayer : public MapLayer {
 public:
  DeepOceanMapLayer(int_fast64_t seed, std::shared_ptr<MapLayer> belowLayer) : MapLayer(seed), belowLayer_(std::move(belowLayer)) {}

  ~DeepOceanMapLayer();

  BiomeGrid GenerateValues(int x, int z, int sizeX, int sizeZ) override;

 private:
  std::shared_ptr<MapLayer> belowLayer_;
};

#endif //EXT_VANILLAGENERATOR_LIB_GENERATOR_BIOMEGRID_DEEPOCEANMAPLAYER_H_
