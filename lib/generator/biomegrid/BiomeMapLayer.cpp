#include "BiomeMapLayer.h"

BiomeGrid BiomeMapLayer::GenerateValues(int x, int z, int sizeX, int sizeZ) {
  BiomeGrid values = belowLayer_->GenerateValues(x, z, sizeX, sizeZ);

  BiomeGrid finalValues;
  for (int i = 0; i < sizeZ; i++) {
    for (int j = 0; j < sizeX; j++) {
      int val = values[j + i * sizeX];
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
            if(isUHC_){
              val = WET[NextInt(static_cast<int>(WET.size()))];
            } else {
              val = DRY_LARGE[NextInt(static_cast<int>(DRY_LARGE.size()))];
            }
            break;
          case 1002:
            val = WARM_LARGE[NextInt(static_cast<int>(WARM_LARGE.size()))];
            break;
          case 1004:
            if(isUHC_){
              val = WARM_LARGE[NextInt(static_cast<int>(WARM_LARGE.size()))];
            } else {
              val = WET_LARGE[NextInt(static_cast<int>(WET_LARGE.size()))];
            }
            break;
          default:
            break;
        }
      }
      finalValues[j + i * sizeX] = val;
    }
  }

  return finalValues;
}

BiomeMapLayer::~BiomeMapLayer() {
  belowLayer_.reset();
}
