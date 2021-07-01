#ifndef EXT_NOISELIB_LIB_GENERATOR_BIOMEGRID_SHOREMAPLAYER_H_
#define EXT_NOISELIB_LIB_GENERATOR_BIOMEGRID_SHOREMAPLAYER_H_

#include <lib/pocketmine/BiomeList.h>
#include "MapLayer.h"

using namespace GridBiome;

class ShoreMapLayer : public MapLayer {
 public:
  ShoreMapLayer(int_fast64_t seed, MapLayer *below_layer) : MapLayer(seed), below_layer_(below_layer) {}

  BiomeGrid GenerateValues(int x, int z, int sizeX, int sizeZ) override;
 private:
  bool OceanContains(int value);

  MapLayer *below_layer_;

  const std::vector<int> OCEANS = {OCEAN, DEEP_OCEAN};
  const std::map<int, int> SPECIAL_SHORES = {
      {EXTREME_HILLS, STONE_BEACH},
      {EXTREME_HILLS_PLUS, STONE_BEACH},
      {EXTREME_HILLS_MOUNTAINS, STONE_BEACH},
      {EXTREME_HILLS_PLUS_MOUNTAINS, STONE_BEACH},
      {ICE_PLAINS, COLD_BEACH},
      {ICE_MOUNTAINS, COLD_BEACH},
      {ICE_PLAINS_SPIKES, COLD_BEACH},
      {COLD_TAIGA, COLD_BEACH},
      {COLD_TAIGA_HILLS, COLD_BEACH},
      {COLD_TAIGA_MOUNTAINS, COLD_BEACH},
      {MUSHROOM_ISLAND, MUSHROOM_SHORE},
      {SWAMPLAND, SWAMPLAND},
      {MESA, MESA},
      {MESA_PLATEAU_FOREST, MESA_PLATEAU_FOREST},
      {MESA_PLATEAU_FOREST_MOUNTAINS, MESA_PLATEAU_FOREST_MOUNTAINS},
      {MESA_PLATEAU, MESA_PLATEAU},
      {MESA_PLATEAU_MOUNTAINS, MESA_PLATEAU_MOUNTAINS},
      {MESA_BRYCE, MESA_BRYCE}
  };
};

#endif //EXT_NOISELIB_LIB_GENERATOR_BIOMEGRID_SHOREMAPLAYER_H_
