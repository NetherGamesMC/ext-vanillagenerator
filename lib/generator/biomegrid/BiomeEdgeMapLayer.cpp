#include <algorithm>
#include "BiomeEdgeMapLayer.h"

GridBiome::BiomeGrid BiomeEdgeMapLayer::GenerateValues(int x, int z, int sizeX, int sizeZ) {
  int gridX = x - 1;
  int gridZ = z - 1;
  int gridSizeX = sizeX + 2;
  int gridSizeZ = sizeZ + 2;
  GridBiome::BiomeGrid values = belowLayer_->GenerateValues(gridX, gridZ, gridSizeX, gridSizeZ);

  GridBiome::BiomeGrid finalValues;
  for (int i = 0; i < sizeZ; i++) {
    for (int j = 0; j < sizeX; j++) {
      // This applies biome large edges using Von Neumann neighborhood
      int centerVal = values[j + 1 + (i + 1) * gridSizeX];
      int val = centerVal;
      for (const auto& entry : EDGES) {
        std::map<int, int> map = entry.first;
        if (map.find(centerVal) != map.end()) {
          int upperVal = values[j + 1 + i * gridSizeX];
          int lowerVal = values[j + 1 + (i + 2) * gridSizeX];
          int leftVal = values[j + (i + 1) * gridSizeX];
          int rightVal = values[j + 2 + (i + 1) * gridSizeX];
          if (entry.second.empty() && (
              map.find(upperVal) == map.end()
                  || map.find(lowerVal) == map.end()
                  || map.find(leftVal) == map.end()
                  || map.find(rightVal) == map.end())) {
            val = map[centerVal];
            break;
          } else if (!entry.second.empty() && (
              std::find(entry.second.begin(), entry.second.end(), upperVal) != entry.second.end()
                  || std::find(entry.second.begin(), entry.second.end(), lowerVal) != entry.second.end()
                  || std::find(entry.second.begin(), entry.second.end(), leftVal) != entry.second.end()
                  || std::find(entry.second.begin(), entry.second.end(), rightVal) != entry.second.end())) {
            val = map[centerVal];
            break;
          }
        }
      }

      finalValues[j + i * sizeX] = val;
    }
  }

  return finalValues;
}

BiomeEdgeMapLayer::~BiomeEdgeMapLayer() {
  belowLayer_.reset();
}
