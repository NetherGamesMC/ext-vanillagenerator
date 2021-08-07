#include <algorithm>
#include "BiomeThinEdgeMapLayer.h"

BiomeGrid BiomeThinEdgeMapLayer::GenerateValues(int x, int z, int sizeX, int sizeZ) {
  int gridX = x - 1;
  int gridZ = z - 1;
  int gridSizeX = sizeX + 2;
  int gridSizeZ = sizeZ + 2;
  BiomeGrid values = belowLayer_->GenerateValues(gridX, gridZ, gridSizeX, gridSizeZ);

  BiomeGrid finalValues;
  for (int i = 0; i < sizeZ; i++) {
    for (int j = 0; j < sizeX; j++) {
      // This applies biome thin edges using Von Neumann neighborhood
      int centerVal = values[j + 1 + (i + 1) * gridSizeX];
      int val = centerVal;
      if (!isUHC_) {
        for (const auto &entry : EDGES) {
          std::map<int, int> map = entry.first;
          if (map.find(centerVal) != map.end()) {
            int upperVal = values[j + 1 + i * gridSizeX];
            int lowerVal = values[j + 1 + (i + 2) * gridSizeX];
            int leftVal = values[j + (i + 1) * gridSizeX];
            int rightVal = values[j + 2 + (i + 1) * gridSizeX];
            if (entry.second.empty() && (
                OceanContains(upperVal) && map.find(upperVal) == map.end()
                    || OceanContains(lowerVal) && map.find(lowerVal) == map.end()
                    || OceanContains(leftVal) && map.find(leftVal) == map.end()
                    || OceanContains(rightVal) && map.find(rightVal) == map.end())) {
              val = map[centerVal];
              break;
            } else if (!entry.second.empty() && (
                OceanContains(upperVal) && EdgesContains(entry.second, upperVal)
                    || OceanContains(lowerVal) && EdgesContains(entry.second, lowerVal)
                    || OceanContains(leftVal) && EdgesContains(entry.second, leftVal)
                    || OceanContains(rightVal) && EdgesContains(entry.second, rightVal))) {
              val = map[centerVal];
              break;
            }
          }
        }
      }

      finalValues[j + i * sizeX] = val;
    }
  }
  return finalValues;
}

bool BiomeThinEdgeMapLayer::OceanContains(int value) {
  return std::find(OCEANS.begin(), OCEANS.end(), value) == OCEANS.end();
}

bool BiomeThinEdgeMapLayer::EdgesContains(std::vector<int> entry, int value) {
  return std::find(entry.begin(), entry.end(), value) == entry.end();
}

BiomeThinEdgeMapLayer::~BiomeThinEdgeMapLayer() {
  belowLayer_.reset();
}
