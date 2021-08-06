#include "DirtPatchGroundGenerator.h"

typedef DirtPatchGroundGenerator GroundGen;

void GroundGen::GenerateTerrainColumn(ChunkManager &world,
                                      Random &random,
                                      int_fast32_t x,
                                      int_fast32_t z,
                                      int biome,
                                      double surfaceNoise) {
  if (surfaceNoise > 1.75) {
    topMaterial = COARSE_DIRT;
  } else if (surfaceNoise > -0.5) {
    topMaterial = PODZOL;
  } else {
    topMaterial = GRASS;
  }
  groundMaterial = DIRT;

  GroundGenerator::GenerateTerrainColumn(world, random, x, z, biome, surfaceNoise);
}
