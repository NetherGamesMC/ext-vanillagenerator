#include "WhittakerMapLayer.h"

BiomeGrid WhittakerMapLayer::GenerateValues(int x, int z, int sizeX, int sizeZ) {
  if (type_ == WARM_WET || type_ == COLD_DRY) {
    return SwapValues(x, z, sizeX, sizeZ);
  }

  return ModifyValues(x, z, sizeX, sizeZ);
}

BiomeGrid WhittakerMapLayer::SwapValues(int x, int z, int sizeX, int sizeZ) {
  int gridX = x - 1;
  int gridZ = z - 1;
  int gridSizeX = sizeX + 2;
  int gridSizeZ = sizeZ + 2;

  BiomeGrid values = belowLayer_->GenerateValues(gridX, gridZ, gridSizeX, gridSizeZ);

  BiomeGrid finalValues;
  Climate climate = climateArray_[type_];
  for (int i = 0; i < sizeZ; i++) {
    for (int j = 0; j < sizeX; j++) {
      int centerVal = values[j + 1 + (i + 1) * gridSizeX];
      if (centerVal == climate.value) {
        int upperVal = values[j + 1 + i * gridSizeX];
        int lowerVal = values[j + 1 + (i + 2) * gridSizeX];
        int leftVal = values[j + (i + 1) * gridSizeX];
        int rightVal = values[j + 2 + (i + 1) * gridSizeX];

        for (int type : climate.crossTypes) {
          if (upperVal == type || lowerVal == type || leftVal == type || rightVal == type) {
            centerVal = climate.finalValue;
            break;
          }
        }
      }

      finalValues[j + i * sizeX] = centerVal;
    }
  }
  return finalValues;
}

BiomeGrid WhittakerMapLayer::ModifyValues(int x, int z, int sizeX, int sizeZ) {
  BiomeGrid values = belowLayer_->GenerateValues(x, z, sizeX, sizeZ);
  BiomeGrid finalValues;
  for (int i = 0; i < sizeZ; i++) {
    for (int j = 0; j < sizeX; j++) {
      int val = values[j + i * sizeX];
      if (val != 0) {
        SetCoordsSeed(x + j, z + i);
        if (NextInt(13) == 0) {
          val += 1000;
        }
      }
      finalValues[j + i * sizeX] = val;
    }
  }
  return finalValues;
}

WhittakerMapLayer::~WhittakerMapLayer() {
  belowLayer_.reset();
}
