#ifndef EXT_NOISELIB_LIB_GENERATOR_BIOMEGRID_MAPLAYER_H_
#define EXT_NOISELIB_LIB_GENERATOR_BIOMEGRID_MAPLAYER_H_

#include <cstdint>
#include <vector>
#include <lib/random/Random.h>
#include <map>

namespace GridBiome {

struct MapLayerPair {
  MapLayerPair *high_resolution;
  MapLayerPair *low_resolution;
};

typedef std::map<int, int> BlockValues;

class MapLayer {
 public:
  explicit MapLayer(int_fast64_t seed) {
    random_.setSeed(seed);
  }

  virtual BlockValues GenerateValues(int x, int z, int size_x, int size_z) = 0;

  auto SetCoordsSeed(int x, int z) -> void;

  auto NextInt(int max) -> int;

 protected:
  Random random_ = Random(0);
};

MapLayerPair initialize(int_fast64_t seed);

}

#endif //EXT_NOISELIB_LIB_GENERATOR_BIOMEGRID_MAPLAYER_H_
