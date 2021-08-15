#ifndef EXT_VANILLAGENERATOR_LIB_GENERATOR_BIOMEGRID_SHOREMAPLAYER_H_
#define EXT_VANILLAGENERATOR_LIB_GENERATOR_BIOMEGRID_SHOREMAPLAYER_H_

#include <lib/objects/constants/BiomeList.h>

#include <utility>
#include "MapLayer.h"

using namespace GridBiome;

class ShoreMapLayer : public MapLayer {
 public:
  ShoreMapLayer(int_fast64_t seed, std::shared_ptr<MapLayer> belowLayer, bool isUHC)
      : MapLayer(seed), belowLayer_(std::move(belowLayer)), isUHC_(isUHC) {}

  ~ShoreMapLayer();

  BiomeGrid GenerateValues(int x, int z, int sizeX, int sizeZ) override;
 private:
  bool OceanContains(int value);

  bool isUHC_;
  std::shared_ptr<MapLayer> belowLayer_;

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

#endif //EXT_VANILLAGENERATOR_LIB_GENERATOR_BIOMEGRID_SHOREMAPLAYER_H_
