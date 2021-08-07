#include <algorithm>
#include "ShoreMapLayer.h"

BiomeGrid ShoreMapLayer::GenerateValues(int x, int z, int sizeX, int sizeZ) {
  int gridX = x - 1;
  int gridZ = z - 1;
  int gridSizeX = sizeX + 2;
  int gridSizeZ = sizeZ + 2;
  BiomeGrid values = belowLayer_->GenerateValues(gridX, gridZ, gridSizeX, gridSizeZ);

  BiomeGrid finalValues;
  for (int i = 0; i < sizeZ; i++) {
    for (int j = 0; j < sizeX; j++) {
      if (isUHC_) {
        int centerVal = values[j + 1 + (i + 1) * gridSizeX];

        finalValues[j + i * sizeX] = centerVal;
      } else {
        // This applies shores using Von Neumann neighborhood
        // it takes a 3x3 grid with a cross shape and analyzes values as follow
        // 0X0
        // XxX
        // 0X0
        // the grid center value decides how we are proceeding:
        // - if it's not ocean and it's surrounded by at least 1 ocean cell
        // it turns the center value into beach.
        int upperVal = values[j + 1 + i * gridSizeX];
        int lowerVal = values[j + 1 + (i + 2) * gridSizeX];
        int leftVal = values[j + (i + 1) * gridSizeX];
        int rightVal = values[j + 2 + (i + 1) * gridSizeX];
        int centerVal = values[j + 1 + (i + 1) * gridSizeX];
        if (!OceanContains(centerVal) && (
            OceanContains(upperVal) || OceanContains(lowerVal)
                || OceanContains(leftVal) || OceanContains(rightVal))) {
          finalValues[j + i * sizeX] = SPECIAL_SHORES.find(centerVal) != SPECIAL_SHORES.end()
                                       ? SPECIAL_SHORES.at(centerVal) : BEACH;
        } else {
          finalValues[j + i * sizeX] = centerVal;
        }
      }
    }
  }
  return finalValues;
}

bool ShoreMapLayer::OceanContains(int value) {
  return std::find(OCEANS.begin(), OCEANS.end(), value) != OCEANS.end();
}

ShoreMapLayer::~ShoreMapLayer() {
  belowLayer_.reset();
}
