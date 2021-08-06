#ifndef EXT_NOISELIB_LIB_GENERATOR_BIOMEGRID_RIVERMAPLAYER_H_
#define EXT_NOISELIB_LIB_GENERATOR_BIOMEGRID_RIVERMAPLAYER_H_

#include <lib/objects/constants/BiomeList.h>

#include <utility>
#include "MapLayer.h"

using namespace GridBiome;

class RiverMapLayer : public MapLayer {
 public:
  RiverMapLayer(int_fast32_t seed,
                std::shared_ptr<MapLayer> map_layer,
                std::shared_ptr<MapLayer> merge_layer,
                bool isUHC)
      : MapLayer(seed), below_layer_(std::move(map_layer)), merge_layer_(std::move(merge_layer)), is_uhc_(isUHC) {}

  ~RiverMapLayer();

  BiomeGrid GenerateValues(int x, int z, int size_x, int size_z) override;

 private:
  BiomeGrid GenerateRivers(int x, int z, int size_x, int size_z);

  BiomeGrid MergeRivers(int x, int z, int size_x, int size_z);

  std::shared_ptr<MapLayer> below_layer_;
  std::shared_ptr<MapLayer> merge_layer_;

  bool is_uhc_;

  const std::vector<int> OCEANS = {OCEAN, DEEP_OCEAN};
  const std::map<int, int> SPECIAL_RIVERS = {
      {ICE_PLAINS, FROZEN_RIVER},
      {MUSHROOM_ISLAND, MUSHROOM_SHORE},
      {MUSHROOM_SHORE, MUSHROOM_SHORE}
  };

  const int CLEAR_VALUE = 0;
  const int RIVER_VALUE = 1;
};

#endif //EXT_NOISELIB_LIB_GENERATOR_BIOMEGRID_RIVERMAPLAYER_H_
