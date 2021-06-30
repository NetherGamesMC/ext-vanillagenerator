#include <algorithm>
#include "RiverMapLayer.h"

BlockValues RiverMapLayer::GenerateValues(int x, int z, int size_x, int size_z) {
  if (merge_layer_ == nullptr) {
    return GenerateRivers(x, z, size_x, size_z);
  }
  return MergeRivers(x, z, size_x, size_z);
}

BlockValues RiverMapLayer::GenerateRivers(int x, int z, int size_x, int size_z) {
  int gridX = x - 1;
  int gridZ = z - 1;
  int gridSizeX = size_x + 2;
  int gridSizeZ = size_z + 2;

  BlockValues values = below_layer_.GenerateValues(gridX, gridZ, gridSizeX, gridSizeZ);
  BlockValues finalValues;
  for (int i = 0; i < size_z; i++) {
    for (int j = 0; j < size_x; j++) {
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
      finalValues[j + i * size_x] = val;
    }
  }
  return finalValues;
}

BlockValues RiverMapLayer::MergeRivers(int x, int z, int size_x, int size_z) {
  BlockValues values = below_layer_.GenerateValues(x, z, size_x, size_z);
  BlockValues mergeValues = merge_layer_.GenerateValues(x, z, size_x, size_z);

  BlockValues finalValues;
  for (int i = 0; i < size_x * size_z; i++) {
    int val = mergeValues[i];
    if (std::find(OCEANS.begin(), OCEANS.end(), mergeValues[i]) != OCEANS.end()) {
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

