#include "SmoothMapLayer.h"

BlockValues SmoothMapLayer::GenerateValues(int x, int z, int size_x, int size_z) {
  int gridX = x - 1;
  int gridZ = z - 1;
  int gridSizeX = size_x + 2;
  int gridSizeZ = size_z + 2;
  BlockValues values = below_layer_->GenerateValues(gridX, gridZ, gridSizeX, gridSizeZ);

  BlockValues finalValues;
  for (int i = 0; i < size_z; i++) {
    for (int j = 0; j < size_x; j++) {
      // This applies smoothing using Von Neumann neighborhood
      // it takes a 3x3 grid with a cross shape and analyzes values as follow
      // 0X0
      // XxX
      // 0X0
      // it is required that we use the same shape that was used for what we
      // want to smooth
      int upperVal = values[j + 1 + i * gridSizeX];
      int lowerVal = values[j + 1 + (i + 2) * gridSizeX];
      int leftVal = values[j + (i + 1) * gridSizeX];
      int rightVal = values[j + 2 + (i + 1) * gridSizeX];
      int centerVal = values[j + 1 + (i + 1) * gridSizeX];
      if (upperVal == lowerVal && leftVal == rightVal) {
        SetCoordsSeed(x + j, z + i);
        centerVal = NextInt(2) == 0 ? upperVal : leftVal;
      } else if (upperVal == lowerVal) {
        centerVal = upperVal;
      } else if (leftVal == rightVal) {
        centerVal = leftVal;
      }
      finalValues[j + i * size_x] = centerVal;
    }
  }
  return finalValues;
}
