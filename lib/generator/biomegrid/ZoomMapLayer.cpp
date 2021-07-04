#include "ZoomMapLayer.h"

int ZoomMapLayer::getNearest(int upperLeftVal, int upperRightVal, int lowerLeftVal, int lowerRightVal) {
  if (zoom_type_ == NORMAL) {
    if (upperRightVal == lowerLeftVal && lowerLeftVal == lowerRightVal) {
      return upperRightVal;
    } else if (upperLeftVal == upperRightVal && upperLeftVal == lowerLeftVal) {
      return upperLeftVal;
    } else if (upperLeftVal == upperRightVal && upperLeftVal == lowerRightVal) {
      return upperLeftVal;
    } else if (upperLeftVal == lowerLeftVal && upperLeftVal == lowerRightVal) {
      return upperLeftVal;
    } else if (upperLeftVal == upperRightVal && lowerLeftVal != lowerRightVal) {
      return upperLeftVal;
    } else if (upperLeftVal == lowerLeftVal && upperRightVal != lowerRightVal) {
      return upperLeftVal;
    } else if (upperLeftVal == lowerRightVal && upperRightVal != lowerLeftVal) {
      return upperLeftVal;
    } else if (upperRightVal == lowerLeftVal && upperLeftVal != lowerRightVal) {
      return upperRightVal;
    } else if (upperRightVal == lowerRightVal && upperLeftVal != lowerLeftVal) {
      return upperRightVal;
    } else if (lowerLeftVal == lowerRightVal && upperLeftVal != upperRightVal) {
      return lowerLeftVal;
    }
  }
  std::vector<int> values = {upperLeftVal, upperRightVal, lowerLeftVal, lowerRightVal};
  return values[NextInt(4)];
}

BiomeGrid ZoomMapLayer::GenerateValues(int x, int z, int size_x, int size_z) {
  int gridX = x >> 1;
  int gridZ = z >> 1;
  int gridSizeX = (size_x >> 1) + 2;
  int gridSizeZ = (size_z >> 1) + 2;
  BiomeGrid values = below_layer_->GenerateValues(gridX, gridZ, gridSizeX, gridSizeZ);

  int zoomSizeX = (gridSizeX - 1) << 1;

  BiomeGrid tmpValues;
  for (int i = 0; i < gridSizeZ - 1; i++) {
    int n = i * 2 * zoomSizeX;
    int upperLeftVal = values[i * gridSizeX];
    int lowerLeftVal = values[(i + 1) * gridSizeX];
    for (int j = 0; j < gridSizeX - 1; j++) {
      SetCoordsSeed((gridX + j) << 1, (gridZ + i) << 1);
      tmpValues[n] = upperLeftVal;
      tmpValues[n + zoomSizeX] = NextInt(2) > 0 ? upperLeftVal : lowerLeftVal;
      int upperRightVal = values[j + 1 + i * gridSizeX];
      int lowerRightVal = values[j + 1 + (i + 1) * gridSizeX];
      tmpValues[n + 1] = NextInt(2) > 0 ? upperLeftVal : upperRightVal;
      tmpValues[n + 1 + zoomSizeX] = getNearest(upperLeftVal, upperRightVal, lowerLeftVal, lowerRightVal);
      upperLeftVal = upperRightVal;
      lowerLeftVal = lowerRightVal;
      n += 2;
    }
  }

  BiomeGrid finalValues;
  for (int i = 0; i < size_z; i++) {
    for (int j = 0; j < size_x; j++) {
      finalValues[j + i * size_x] = tmpValues[j + (i + (z & 1)) * zoomSizeX + (x & 1)];
    }
  }

  return finalValues;
}

ZoomMapLayer::~ZoomMapLayer() {
  delete random_;

  below_layer_.reset();
}
