#include "NoiseMapLayer.h"

BiomeGrid NoiseMapLayer::GenerateValues(int x, int z, int sizeX, int sizeZ) {
  BiomeGrid values;
  for (int i = 0; i < sizeZ; ++i) {
    for (int j = 0; j < sizeX; ++j) {
      double noise = noiseGen_.Noise(x + j, z + i, 0.175, 0.8, true) * 4.0;
      int val;
      if (noise >= 0.05) {
        val = noise <= 0.2 ? 3 : 2;
      } else {
        SetCoordsSeed(x + j, z + i);

        val = NextInt(2) == 0 ? 3 : 0;
      }

      values.insert({j + i * sizeX, val});
    }
  }

  return values;
}