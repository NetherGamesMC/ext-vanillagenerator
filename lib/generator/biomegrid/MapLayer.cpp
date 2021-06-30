#include "MapLayer.h"

namespace GridBiome {

int MapLayer::NextInt(int max) {
  return static_cast<int>(random_.nextInt(max));
}

void MapLayer::SetCoordsSeed(int x, int z) {
  random_.setSeed(x * random_.nextInt() + z * random_.nextInt() ^ random_.getSeed());
}