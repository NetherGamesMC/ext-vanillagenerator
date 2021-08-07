#include "RarePlainsMapLayer.h"

BiomeGrid RarePlainsMapLayer::GenerateValues(int x, int z, int sizeX, int sizeZ) {
  int gridX = x - 1;
  int gridZ = z - 1;
  int gridSizeX = sizeX + 2;
  int gridSizeZ = sizeZ + 2;

  BiomeGrid values = belowLayer_->GenerateValues(gridX, gridZ, gridSizeX, gridSizeZ);

  BiomeGrid finalValues;
  for (int i = 0; i < sizeZ; i++) {
    for (int j = 0; j < sizeX; j++) {
      SetCoordsSeed(x + j, z + i);
      int centerValue = values[j + 1 + (i + 1) * gridSizeX];
      if (NextInt(57) == 0 && RARE_PLAINS.find(centerValue) != RARE_PLAINS.end()) {
        centerValue = RARE_PLAINS.at(centerValue);
      }
      finalValues[j + i * sizeX] = centerValue;
    }
  }

  return finalValues;
}

RarePlainsMapLayer::~RarePlainsMapLayer() {
  belowLayer_.reset();
}
