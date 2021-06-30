#ifndef EXT_NOISELIB_LIB_GENERATOR_BIOMEGRID_BIOMEMAPLAYER_H_
#define EXT_NOISELIB_LIB_GENERATOR_BIOMEGRID_BIOMEMAPLAYER_H_

#include <lib/pocketmine/BiomeList.h>
#include "MapLayer.h"

using namespace GridBiome;

class BiomeMapLayer : public MapLayer {
 public:
  BiomeMapLayer(int_fast64_t seed, MapLayer &belowLayer) : MapLayer(seed), below_layer_(belowLayer) {}

  BlockValues GenerateValues(int x, int z, int size_x, int size_z) override;
 private:
  MapLayer &below_layer_;

  const std::vector<int> WARM = {DESERT, DESERT, DESERT, SAVANNA, SAVANNA, PLAINS};
  const std::vector<int> WET = {PLAINS, PLAINS, FOREST, BIRCH_FOREST, ROOFED_FOREST, EXTREME_HILLS, SWAMPLAND};
  const std::vector<int> DRY = {PLAINS, FOREST, TAIGA, EXTREME_HILLS};
  const std::vector<int> COLD = {ICE_PLAINS, ICE_PLAINS, COLD_TAIGA};
  const std::vector<int> WARM_LARGE = {MESA_PLATEAU_FOREST, MESA_PLATEAU_FOREST, MESA_PLATEAU};
  const std::vector<int> DRY_LARGE = {MEGA_TAIGA};
  const std::vector<int> WET_LARGE = {JUNGLE};
}

#endif //EXT_NOISELIB_LIB_GENERATOR_BIOMEGRID_BIOMEMAPLAYER_H_
