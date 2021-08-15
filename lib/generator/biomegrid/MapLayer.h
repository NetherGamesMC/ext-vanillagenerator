#ifndef EXT_VANILLAGENERATOR_LIB_GENERATOR_BIOMEGRID_MAPLAYER_H_
#define EXT_VANILLAGENERATOR_LIB_GENERATOR_BIOMEGRID_MAPLAYER_H_

#include <cstdio>
#include <cstdint>
#include <vector>
#include <lib/objects/random/Random.h>
#include <map>
#include <memory>

namespace GridBiome {

typedef std::map<int, int> BiomeGrid;

class MapLayer {
 public:
  explicit MapLayer(int_fast64_t seed): seed_(seed), random_(seed) {}

  virtual BiomeGrid GenerateValues(int x, int z, int sizeX, int sizeZ) = 0;

  auto SetCoordsSeed(int x, int z) -> void;

  auto NextInt(int max) -> int;

 protected:
  int_fast64_t seed_;
  Random random_;
};

struct MapLayerPair {
  std::shared_ptr<MapLayer> highResolution;
  std::shared_ptr<MapLayer> lowResolution;
};

MapLayerPair Initialize(int_fast64_t seed, bool isUHC);

}

#endif //EXT_VANILLAGENERATOR_LIB_GENERATOR_BIOMEGRID_MAPLAYER_H_
