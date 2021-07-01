#ifndef EXT_NOISELIB_LIB_GENERATOR_BIOMEGRID_BIOMEEDGEMAPLAYER_H_
#define EXT_NOISELIB_LIB_GENERATOR_BIOMEGRID_BIOMEEDGEMAPLAYER_H_

#include <lib/pocketmine/BiomeList.h>
#include "MapLayer.h"

class BiomeEdgeMapLayer : public GridBiome::MapLayer {
 public:
  BiomeEdgeMapLayer(int_fast64_t seed, MapLayer *below_layer) : MapLayer(seed), below_layer_(below_layer) {}

  GridBiome::BiomeGrid GenerateValues(int x, int z, int sizeX, int sizeZ) override;
 private:
  MapLayer *below_layer_;

  const std::map<std::map<int, int>, std::vector<int>> EDGES = {
      {{{MESA_PLATEAU_FOREST, MESA}, {MESA_PLATEAU, MESA}}, {}},
      {{{MEGA_TAIGA, TAIGA}}, {}},
      {{{DESERT, EXTREME_HILLS_PLUS}}, {ICE_PLAINS}},
      {{{SWAMPLAND, PLAINS}}, {DESERT, COLD_TAIGA, ICE_PLAINS}},
      {{{SWAMPLAND, JUNGLE_EDGE}}, {JUNGLE}}
  };
};

#endif //EXT_NOISELIB_LIB_GENERATOR_BIOMEGRID_BIOMEEDGEMAPLAYER_H_
