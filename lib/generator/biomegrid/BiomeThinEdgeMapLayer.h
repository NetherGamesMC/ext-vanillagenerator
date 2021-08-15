#ifndef EXT_VANILLAGENERATOR_LIB_GENERATOR_BIOMEGRID_BIOMETHINEDGEMAPLAYER_H_
#define EXT_VANILLAGENERATOR_LIB_GENERATOR_BIOMEGRID_BIOMETHINEDGEMAPLAYER_H_

#include <lib/objects/constants/BiomeList.h>

#include <utility>
#include "MapLayer.h"

using namespace GridBiome;

class BiomeThinEdgeMapLayer : public MapLayer {
 public:
  BiomeThinEdgeMapLayer(int_fast64_t seed, std::shared_ptr<MapLayer> belowLayer, bool isUHC) : MapLayer(seed), belowLayer_(std::move(belowLayer)), isUHC_(isUHC) {}

  ~BiomeThinEdgeMapLayer();

  BiomeGrid GenerateValues(int x, int z, int sizeX, int sizeZ) override;
 private:
  bool OceanContains(int value);

  static bool EdgesContains(std::vector<int> entry, int value);

  bool isUHC_;
  std::shared_ptr<MapLayer> belowLayer_;

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
};

#endif //EXT_VANILLAGENERATOR_LIB_GENERATOR_BIOMEGRID_BIOMETHINEDGEMAPLAYER_H_
