#include <algorithm>
#include "BiomeVariationMapLayer.h"

BiomeGrid BiomeVariationMapLayer::GenerateValues(int x, int z, int sizeX, int sizeZ) {
  if (variationLayer_ == nullptr) {
    return GenerateRandomValues(x, z, sizeX, sizeZ);
  }

  return MergeValues(x, z, sizeX, sizeZ);
}

BiomeGrid BiomeVariationMapLayer::GenerateRandomValues(int x, int z, int sizeX, int sizeZ) {
  BiomeGrid values = belowLayer_->GenerateValues(x, z, sizeX, sizeZ);

  BiomeGrid finalValues;
  for (int i = 0; i < sizeZ; i++) {
    for (int j = 0; j < sizeX; j++) {
      int val = values[j + i * sizeX];
      if (val > 0) {
        SetCoordsSeed(x + j, z + i);
        val = NextInt(30) + 2;
      }
      finalValues[j + i * sizeX] = val;
    }
  }
  return finalValues;
}

BiomeGrid BiomeVariationMapLayer::MergeValues(int x, int z, int sizeX, int sizeZ) {
  int gridX = x - 1;
  int gridZ = z - 1;
  int gridSizeX = sizeX + 2;
  int gridSizeZ = sizeZ + 2;

  BiomeGrid values = belowLayer_->GenerateValues(gridX, gridZ, gridSizeX, gridSizeZ);
  BiomeGrid variationValues = variationLayer_->GenerateValues(gridX, gridZ, gridSizeX, gridSizeZ);

  BiomeGrid finalValues;
  for (int i = 0; i < sizeZ; i++) {
    for (int j = 0; j < sizeX; j++) {
      SetCoordsSeed(x + j, z + i);
      int centerValue = values[j + 1 + (i + 1) * gridSizeX];
      int variationValue = variationValues[j + 1 + (i + 1) * gridSizeX];
      if (centerValue != 0 && variationValue == 3 && centerValue < 128) {
        std::vector<int> data = ALL_BIOMES;
        finalValues[j + i * sizeX] =
            std::find(data.begin(), data.end(), centerValue + 128) != data.end() ? centerValue + 128 : centerValue;
      } else if (variationValue == 2 || NextInt(3) == 0) {
        int val = centerValue;
        if (variations_.find(centerValue) != variations_.end()) {
          std::vector<int> var = variations_.at(centerValue);

          val = var[NextInt(static_cast<int>(var.size()))];
        } else if (!is_uhc_ && centerValue == DEEP_OCEAN && NextInt(3) == 0) {
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
          finalValues[j + i * sizeX] = count < 3 ? centerValue : val;
        } else {
          finalValues[j + i * sizeX] = val;
        }
      } else {
        finalValues[j + i * sizeX] = centerValue;
      }
    }
  }
  return finalValues;
}

BiomeVariationMapLayer::~BiomeVariationMapLayer() {
  belowLayer_.reset();
  variationLayer_.reset();
}
