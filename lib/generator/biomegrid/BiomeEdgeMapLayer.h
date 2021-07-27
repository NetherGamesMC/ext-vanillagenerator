#ifndef EXT_NOISELIB_LIB_GENERATOR_BIOMEGRID_BIOMEEDGEMAPLAYER_H_
#define EXT_NOISELIB_LIB_GENERATOR_BIOMEGRID_BIOMEEDGEMAPLAYER_H_

#include <lib/pocketmine/BiomeList.h>

#include <utility>
#include "MapLayer.h"

class BiomeEdgeMapLayer : public GridBiome::MapLayer {
 public:
  BiomeEdgeMapLayer(int_fast64_t seed, std::shared_ptr<MapLayer> below_layer, bool isUHC)
      : MapLayer(seed), below_layer_(std::move(below_layer)) {
    if (isUHC) {
      EDGES = {
          {{{MESA_PLATEAU_FOREST, MESA}, {MESA_PLATEAU, MESA}}, {}},
          {{{DESERT, EXTREME_HILLS_PLUS}}, {ICE_PLAINS}},
      };
    } else {
      EDGES = {
          {{{MESA_PLATEAU_FOREST, MESA}, {MESA_PLATEAU, MESA}}, {}},
          {{{MEGA_TAIGA, TAIGA}}, {}},
          {{{DESERT, EXTREME_HILLS_PLUS}}, {ICE_PLAINS}},
          {{{SWAMPLAND, PLAINS}}, {DESERT, COLD_TAIGA, ICE_PLAINS}},
          {{{SWAMPLAND, JUNGLE_EDGE}}, {JUNGLE}}
      };
    }
  }

  ~BiomeEdgeMapLayer();

  GridBiome::BiomeGrid GenerateValues(int x, int z, int sizeX, int sizeZ) override;
 private:
  std::shared_ptr<MapLayer> below_layer_;

  std::map<std::map<int, int>, std::vector<int>> EDGES;
};

#endif //EXT_NOISELIB_LIB_GENERATOR_BIOMEGRID_BIOMEEDGEMAPLAYER_H_
