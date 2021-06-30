#ifndef EXT_NOISELIB_LIB_GENERATOR_BIOMEGRID_BIOMETHINEDGEMAPLAYER_H_
#define EXT_NOISELIB_LIB_GENERATOR_BIOMEGRID_BIOMETHINEDGEMAPLAYER_H_

#include <lib/pocketmine/BiomeList.h>
#include "MapLayer.h"

using namespace GridBiome;

class BiomeThinEdgeMapLayer : public MapLayer {
 public:
  BiomeThinEdgeMapLayer(int_fast64_t seed, MapLayer &below_layer) : MapLayer(seed), below_layer_(below_layer) {}

  BlockValues GenerateValues(int x, int z, int sizeX, int sizeZ) override;
 private:
  bool OceanContains(int value);

  static bool EdgesContains(std::vector<int> entry, int value);

  MapLayer &below_layer_;

  const std::vector<int> OCEANS = {OCEAN, DEEP_OCEAN};
  const std::map<int, int> MESA_EDGES{
      {MESA, DESERT},
      {MESA_BRYCE, DESERT},
      {MESA_PLATEAU_FOREST, DESERT},
      {MESA_PLATEAU_FOREST_MOUNTAINS, DESERT},
      {MESA_PLATEAU, DESERT},
      {MESA_PLATEAU_MOUNTAINS, DESERT}
  };
  const std::map<int, int> JUNGLE_EDGES = {
      {JUNGLE, JUNGLE_EDGE},
      {JUNGLE_HILLS, JUNGLE_EDGE},
      {JUNGLE_MOUNTAINS, JUNGLE_EDGE},
      {JUNGLE_EDGE_MOUNTAINS, JUNGLE_EDGE}
  };
  const std::map<std::map<int, int>, std::vector<int>> EDGES = {
      {MESA_EDGES, {}},
      {JUNGLE_EDGES, {JUNGLE, JUNGLE_HILLS, JUNGLE_MOUNTAINS, JUNGLE_EDGE_MOUNTAINS, FOREST, TAIGA}}
  };
}

#endif //EXT_NOISELIB_LIB_GENERATOR_BIOMEGRID_BIOMETHINEDGEMAPLAYER_H_
