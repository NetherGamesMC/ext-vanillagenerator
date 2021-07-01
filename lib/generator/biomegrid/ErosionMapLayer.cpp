#include "ErosionMapLayer.h"

BiomeGrid ErosionMapLayer::GenerateValues(int x, int z, int size_x, int size_z) {
  int gridX = x - 1;
  int gridZ = z - 1;
  int gridSizeX = size_x + 2;
  int gridSizeZ = size_z + 2;

  BiomeGrid values = below_layer_->GenerateValues(gridX, gridZ, gridSizeX, gridSizeZ);
  BiomeGrid finalValues;
  for (int i = 0; i < size_z; i++) {
    for (int j = 0; j < size_x; j++) {
      // This applies erosion using Rotated Von Neumann neighborhood
      // it takes a 3x3 grid with a cross shape and analyzes values as follow
      // X0X
      // 0X0
      // X0X
      // the grid center value decides how we are proceeding:
      // - if it's land and it's surrounded by at least 1 ocean cell there are 4/5 chances
      // to proceed to land weathering, and 1/5 chance to spread some land.
      // - if it's ocean and it's surrounded by at least 1 land cell, there are 2/3
      // chances to proceed to land weathering, and 1/3 chance to spread some land.
      int upperLeftVal = values[j + i * gridSizeX];
      int lowerLeftVal = values[j + (i + 2) * gridSizeX];
      int upperRightVal = values[j + 2 + i * gridSizeX];
      int lowerRightVal = values[j + 2 + (i + 2) * gridSizeX];
      int centerVal = values[j + 1 + (i + 1) * gridSizeX];

      SetCoordsSeed(x + j, z + i);
      if (centerVal != 0 && (upperLeftVal == 0 || upperRightVal == 0 || lowerLeftVal == 0
          || lowerRightVal == 0)) {
        finalValues[j + i * size_x] = NextInt(5) == 0 ? 0 : centerVal;
      } else if (centerVal == 0 && (upperLeftVal != 0 || upperRightVal != 0
          || lowerLeftVal != 0 || lowerRightVal != 0)) {
        if (NextInt(3) == 0) {
          finalValues[j + i * size_x] = upperLeftVal;
        } else {
          finalValues[j + i * size_x] = 0;
        }
      } else {
        finalValues[j + i * size_x] = centerVal;
      }
    }
  }
  return finalValues;
}
