#ifndef EXT_NOISELIB_LIB_GENERATOR_BIOMEGRID_RIVERMAPLAYER_H_
#define EXT_NOISELIB_LIB_GENERATOR_BIOMEGRID_RIVERMAPLAYER_H_

#include <lib/pocketmine/BiomeList.h>
#include "MapLayer.h"

using namespace GridBiome;

class RiverMapLayer : public MapLayer {
 public:
  RiverMapLayer(int_fast64_t seed, MapLayer &map_layer, MapLayer *merge_layer)
  : MapLayer(seed), below_layer_(map_layer), merge_layer_(merge_layer) {}

  BlockValues GenerateValues(int x, int z, int size_x, int size_z) override;

 private:
  BlockValues GenerateRivers(int x, int z, int size_x, int size_z);

  BlockValues MergeRivers(int x, int z, int size_x, int size_z);

  MapLayer &below_layer_;
  MapLayer *merge_layer_;

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
