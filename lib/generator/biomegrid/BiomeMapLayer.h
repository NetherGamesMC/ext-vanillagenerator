#ifndef EXT_VANILLAGENERATOR_LIB_GENERATOR_BIOMEGRID_BIOMEMAPLAYER_H_
#define EXT_VANILLAGENERATOR_LIB_GENERATOR_BIOMEGRID_BIOMEMAPLAYER_H_

#include <lib/objects/constants/BiomeList.h>

#include <utility>
#include "MapLayer.h"

using namespace GridBiome;

class BiomeMapLayer : public MapLayer {
 public:
  BiomeMapLayer(int_fast64_t seed, std::shared_ptr<MapLayer> belowLayer, bool isUHC) : MapLayer(seed), belowLayer_(std::move(belowLayer)), isUHC_(isUHC) {
    if (isUHC) {
      WET = {PLAINS, PLAINS, FOREST, BIRCH_FOREST, ROOFED_FOREST, EXTREME_HILLS};
    }
  }

  ~BiomeMapLayer();

  BiomeGrid GenerateValues(int x, int z, int size_x, int size_z) override;
 private:
  bool isUHC_ = false;
  std::shared_ptr<MapLayer> belowLayer_;

  std::vector<int> WET = {PLAINS, PLAINS, FOREST, BIRCH_FOREST, ROOFED_FOREST, EXTREME_HILLS, SWAMPLAND};
  const std::vector<int> WARM = {DESERT, DESERT, DESERT, SAVANNA, SAVANNA, PLAINS};
  const std::vector<int> DRY = {PLAINS, FOREST, TAIGA, EXTREME_HILLS};
  const std::vector<int> COLD = {ICE_PLAINS, ICE_PLAINS, COLD_TAIGA};
  const std::vector<int> WARM_LARGE = {MESA_PLATEAU_FOREST, MESA_PLATEAU_FOREST, MESA_PLATEAU};
  const std::vector<int> DRY_LARGE = {MEGA_TAIGA};
  const std::vector<int> WET_LARGE = {JUNGLE};
};

#endif //EXT_VANILLAGENERATOR_LIB_GENERATOR_BIOMEGRID_BIOMEMAPLAYER_H_
