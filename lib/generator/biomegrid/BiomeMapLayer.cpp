#include "BiomeMapLayer.h"

BiomeGrid BiomeMapLayer::GenerateValues(int x, int z, int size_x, int size_z) {
  BiomeGrid values = below_layer_->GenerateValues(x, z, size_x, size_z);

  BiomeGrid finalValues;
  for (int i = 0; i < size_z; i++) {
    for (int j = 0; j < size_x; j++) {
      int val = values[j + i * size_x];
      if (val != 0) {
        SetCoordsSeed(x + j, z + i);
        switch (val) {
          case 1:
            val = DRY[NextInt(static_cast<int>(DRY.size()))];
            break;
          case 2:
            val = WARM[NextInt(static_cast<int>(WARM.size()))];
            break;
          case 3:
          case 1003:
            val = COLD[NextInt(static_cast<int>(COLD.size()))];
            break;
          case 4:
            val = WET[NextInt(static_cast<int>(WET.size()))];
            break;
          case 1001:
            val = DRY_LARGE[NextInt(static_cast<int>(DRY_LARGE.size()))];
            break;
          case 1002:
            val = WARM_LARGE[NextInt(static_cast<int>(WARM_LARGE.size()))];
            break;
          case 1004:
            val = WET_LARGE[NextInt(static_cast<int>(WET_LARGE.size()))];
            break;
          default:
            break;
        }
      }
      finalValues[j + i * size_x] = val;
    }
  }

  return finalValues;
}

BiomeMapLayer::~BiomeMapLayer() {
  delete random_;

  below_layer_.reset();
}
