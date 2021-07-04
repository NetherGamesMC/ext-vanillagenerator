#ifndef EXT_NOISELIB_LIB_GENERATOR_BIOMEGRID_MAPLAYER_H_
#define EXT_NOISELIB_LIB_GENERATOR_BIOMEGRID_MAPLAYER_H_

#include <cstdio>
#include <cstdint>
#include <vector>
#include <lib/random/Random.h>
#include <map>
#include <memory>

namespace GridBiome {

typedef std::map<int, int> BiomeGrid;

class MapLayer {
 public:
  explicit MapLayer(int_fast64_t seed): seed_(seed) {
   random_ = new Random(seed);
  }

  virtual BiomeGrid GenerateValues(int x, int z, int size_x, int size_z) = 0;

  auto SetCoordsSeed(int x, int z) -> void;

  auto NextInt(int max) -> int;

 protected:
  int_fast64_t seed_;
  Random *random_;
};

struct MapLayerPair {
  std::shared_ptr<MapLayer> high_resolution;
  std::shared_ptr<MapLayer> low_resolution;
};

MapLayerPair initialize(int_fast64_t seed);

}

#endif //EXT_NOISELIB_LIB_GENERATOR_BIOMEGRID_MAPLAYER_H_
