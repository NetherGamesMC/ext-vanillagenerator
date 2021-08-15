#ifndef EXT_VANILLAGENERATOR_LIB_GENERATOR_BIOMEGRID_RIVERMAPLAYER_H_
#define EXT_VANILLAGENERATOR_LIB_GENERATOR_BIOMEGRID_RIVERMAPLAYER_H_

#include <lib/objects/constants/BiomeList.h>

#include <utility>
#include "MapLayer.h"

using namespace GridBiome;

class RiverMapLayer : public MapLayer {
 public:
  RiverMapLayer(int_fast64_t seed,
                std::shared_ptr<MapLayer> mapLayer,
                std::shared_ptr<MapLayer> mergeLayer,
                bool isUHC)
      : MapLayer(seed), belowLayer_(std::move(mapLayer)), mergeLayer_(std::move(mergeLayer)), isUHC_(isUHC) {}

  ~RiverMapLayer();

  BiomeGrid GenerateValues(int x, int z, int sizeX, int sizeZ) override;

 private:
  BiomeGrid GenerateRivers(int x, int z, int sizeX, int sizeZ);

  BiomeGrid MergeRivers(int x, int z, int sizeX, int sizeZ);

  std::shared_ptr<MapLayer> belowLayer_;
  std::shared_ptr<MapLayer> mergeLayer_;

  bool isUHC_;

  const std::vector<int> OCEANS = {OCEAN, DEEP_OCEAN};
  const std::map<int, int> SPECIAL_RIVERS = {
      {ICE_PLAINS, FROZEN_RIVER},
      {MUSHROOM_ISLAND, MUSHROOM_SHORE},
      {MUSHROOM_SHORE, MUSHROOM_SHORE}
  };

  const int CLEAR_VALUE = 0;
  const int RIVER_VALUE = 1;
};

#endif //EXT_VANILLAGENERATOR_LIB_GENERATOR_BIOMEGRID_RIVERMAPLAYER_H_
