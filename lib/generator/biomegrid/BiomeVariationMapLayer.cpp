#include <algorithm>
#include "BiomeVariationMapLayer.h"

BiomeGrid BiomeVariationMapLayer::GenerateValues(int x, int z, int size_x, int size_z) {
  if (variation_layer_ == nullptr) {
    return GenerateRandomValues(x, z, size_x, size_z);
  }

  return MergeValues(x, z, size_x, size_z);
}

BiomeGrid BiomeVariationMapLayer::GenerateRandomValues(int x, int z, int size_x, int size_z) {
  BiomeGrid values = below_layer_->GenerateValues(x, z, size_x, size_z);

  BiomeGrid finalValues;
  for (int i = 0; i < size_z; i++) {
    for (int j = 0; j < size_x; j++) {
      int val = values[j + i * size_x];
      if (val > 0) {
        SetCoordsSeed(x + j, z + i);
        val = NextInt(30) + 2;
      }
      finalValues[j + i * size_x] = val;
    }
  }
  return finalValues;
}

BiomeGrid BiomeVariationMapLayer::MergeValues(int x, int z, int size_x, int size_z) {
  int gridX = x - 1;
  int gridZ = z - 1;
  int gridSizeX = size_x + 2;
  int gridSizeZ = size_z + 2;

  BiomeGrid values = below_layer_->GenerateValues(gridX, gridZ, gridSizeX, gridSizeZ);
  BiomeGrid variationValues = variation_layer_->GenerateValues(gridX, gridZ, gridSizeX, gridSizeZ);

  BiomeGrid finalValues;
  for (int i = 0; i < size_z; i++) {
    for (int j = 0; j < size_x; j++) {
      SetCoordsSeed(x + j, z + i);
      int centerValue = values[j + 1 + (i + 1) * gridSizeX];
      int variationValue = variationValues[j + 1 + (i + 1) * gridSizeX];
      if (centerValue != 0 && variationValue == 3 && centerValue < 128) {
        std::vector<int> data = ALL_BIOMES;
        finalValues[j + i * size_x] =
            std::find(data.begin(), data.end(), centerValue + 128) != data.end() ? centerValue + 128 : centerValue;
      } else if (variationValue == 2 || NextInt(3) == 0) {
        int val = centerValue;
        if (variations_.find(centerValue) != variations_.end()) {
          std::vector<int> var = variations_.at(centerValue);

          val = var[NextInt(static_cast<int>(var.size()))];
        } else if (centerValue == DEEP_OCEAN && NextInt(3) == 0) {
          val = islands_[NextInt(static_cast<int>(islands_.size()))];
        }

        if (variationValue == 2 && val != centerValue) {
          std::vector<int> data = ALL_BIOMES;
          val = std::find(data.begin(), data.end(), val + 128) != data.end() ? val + 128 : centerValue;
        }

        if (val != centerValue) {
          int count = 0;
          if (values[j + 1 + i * gridSizeX] == centerValue) { // upper value
            count++;
          }
          if (values[j + 1 + (i + 2) * gridSizeX] == centerValue) { // lower value
            count++;
          }
          if (values[j + (i + 1) * gridSizeX] == centerValue) { // left value
            count++;
          }
          if (values[j + 2 + (i + 1) * gridSizeX] == centerValue) { // right value
            count++;
          }
          // spread mountains if not too close from an edge
          finalValues[j + i * size_x] = count < 3 ? centerValue : val;
        } else {
          finalValues[j + i * size_x] = val;
        }
      } else {
        finalValues[j + i * size_x] = centerValue;
      }
    }
  }
  return finalValues;
}