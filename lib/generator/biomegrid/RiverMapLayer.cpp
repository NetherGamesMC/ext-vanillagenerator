#include <algorithm>
#include "RiverMapLayer.h"

BiomeGrid RiverMapLayer::GenerateValues(int x, int z, int sizeX, int sizeZ) {
  if (mergeLayer_ == nullptr) {
    return GenerateRivers(x, z, sizeX, sizeZ);
  }
  return MergeRivers(x, z, sizeX, sizeZ);
}

BiomeGrid RiverMapLayer::GenerateRivers(int x, int z, int sizeX, int sizeZ) {
  int gridX = x - 1;
  int gridZ = z - 1;
  int gridSizeX = sizeX + 2;
  int gridSizeZ = sizeZ + 2;

  BiomeGrid values = belowLayer_->GenerateValues(gridX, gridZ, gridSizeX, gridSizeZ);
  BiomeGrid finalValues;
  for (int i = 0; i < sizeZ; i++) {
    for (int j = 0; j < sizeX; j++) {
      // This applies rivers using Von Neumann neighborhood
      int centerVal = values[j + 1 + (i + 1) * gridSizeX] & 1;
      int upperVal = values[j + 1 + i * gridSizeX] & 1;
      int lowerVal = values[j + 1 + (i + 2) * gridSizeX] & 1;
      int leftVal = values[j + (i + 1) * gridSizeX] & 1;
      int rightVal = values[j + 2 + (i + 1) * gridSizeX] & 1;
      int val = CLEAR_VALUE;
      if (centerVal != upperVal || centerVal != lowerVal || centerVal != leftVal || centerVal != rightVal) {
        val = RIVER_VALUE;
      }
      finalValues[j + i * sizeX] = val;
    }
  }
  return finalValues;
}

BiomeGrid RiverMapLayer::MergeRivers(int x, int z, int sizeX, int sizeZ) {
  BiomeGrid values = belowLayer_->GenerateValues(x, z, sizeX, sizeZ);
  BiomeGrid mergeValues = mergeLayer_->GenerateValues(x, z, sizeX, sizeZ);

  BiomeGrid finalValues;
  for (int i = 0; i < sizeX * sizeZ; i++) {
    int val = mergeValues[i];
    if (!isUHC_ && std::find(OCEANS.begin(), OCEANS.end(), mergeValues[i]) != OCEANS.end()) {
      val = mergeValues[i];
    } else if (values[i] == RIVER_VALUE) {
      if (SPECIAL_RIVERS.find(mergeValues[i]) != SPECIAL_RIVERS.end()) {
        val = SPECIAL_RIVERS.at(mergeValues[i]);
      } else {
        val = RIVER;
      }
    }

    finalValues[i] = val;
  }

  return finalValues;
}

RiverMapLayer::~RiverMapLayer() {
  belowLayer_.reset();
  mergeLayer_.reset();
}

