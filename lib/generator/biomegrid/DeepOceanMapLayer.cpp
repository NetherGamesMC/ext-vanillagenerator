#include <lib/objects/constants/BiomeList.h>
#include "DeepOceanMapLayer.h"

BiomeGrid DeepOceanMapLayer::GenerateValues(int x, int z, int size_x, int size_z) {
  int gridX = x - 1;
  int gridZ = z - 1;
  int gridSizeX = size_x + 2;
  int gridSizeZ = size_z + 2;

  BiomeGrid values = below_layer_->GenerateValues(gridX, gridZ, gridSizeX, gridSizeZ);
  BiomeGrid finalValues;

  for (int i = 0; i < size_z; i++) {
    for (int j = 0; j < size_x; j++) {
      // This applies deep oceans using Von Neumann neighborhood
      // it takes a 3x3 grid with a cross shape and analyzes values as follow
      // 0X0
      // XxX
      // 0X0
      // the grid center value decides how we are proceeding:
      // - if it's ocean and it's surrounded by 4 ocean cells we spread deep ocean.
      int centerVal = values[j + 1 + (i + 1) * gridSizeX];
      if (centerVal == 0) {
        int upperVal = values[j + 1 + i * gridSizeX];
        int lowerVal = values[j + 1 + (i + 2) * gridSizeX];
        int leftVal = values[j + (i + 1) * gridSizeX];
        int rightVal = values[j + 2 + (i + 1) * gridSizeX];
        if (upperVal == 0 && lowerVal == 0 && leftVal == 0 && rightVal == 0) {
          SetCoordsSeed(x + j, z + i);
          finalValues[j + i * size_x] =
              NextInt(100) == 0 ? MUSHROOM_ISLAND : DEEP_OCEAN;
        } else {
          finalValues[j + i * size_x] = centerVal;
        }
      } else {
        finalValues[j + i * size_x] = centerVal;
      }
    }
  }

  return finalValues;
}

DeepOceanMapLayer::~DeepOceanMapLayer() {
  delete random_;

  below_layer_.reset();
}
