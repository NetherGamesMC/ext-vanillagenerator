#ifndef EXT_VANILLAGENERATOR_LIB_GENERATOR_BIOMEGRID_RAREPLAINSMAPLAYER_H_
#define EXT_VANILLAGENERATOR_LIB_GENERATOR_BIOMEGRID_RAREPLAINSMAPLAYER_H_

#include <lib/objects/constants/BiomeList.h>

#include <utility>
#include "MapLayer.h"

using namespace GridBiome;

class RarePlainsMapLayer : public MapLayer {
 public:
  RarePlainsMapLayer(int_fast64_t seed, std::shared_ptr<MapLayer> belowLayer) : MapLayer(seed), belowLayer_(std::move(belowLayer)) {}

  ~RarePlainsMapLayer();

  BiomeGrid GenerateValues(int x, int z, int sizeX, int sizeZ) override;
 private:
  std::shared_ptr<MapLayer> belowLayer_;

  const std::map<int, int> RARE_PLAINS = {{PLAINS, SUNFLOWER_PLAINS}};
};

#endif //EXT_VANILLAGENERATOR_LIB_GENERATOR_BIOMEGRID_RAREPLAINSMAPLAYER_H_
