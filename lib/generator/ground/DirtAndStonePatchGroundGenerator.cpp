#include "DirtAndStonePatchGroundGenerator.h"

typedef DirtAndStonePatchGroundGenerator GroundGen;

void GroundGen::GenerateTerrainColumn(ChunkManager &world,
                                      Random &random,
                                      int_fast64_t x,
                                      int_fast64_t z,
                                      int biome,
                                      double surfaceNoise) {
  if (surfaceNoise > 1.75) {
    topMaterial = STONE;
    groundMaterial = STONE;
  } else if (surfaceNoise > -0.5) {
    topMaterial = COARSE_DIRT;
    groundMaterial = DIRT;
  } else {
    topMaterial = GRASS;
    groundMaterial = DIRT;
  }

  GroundGenerator::GenerateTerrainColumn(world, random, x, z, biome, surfaceNoise);
}
