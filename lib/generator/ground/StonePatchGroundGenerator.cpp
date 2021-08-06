#include "StonePatchGroundGenerator.h"

void StonePatchGroundGenerator::GenerateTerrainColumn(ChunkManager &world,
                                                 Random &random,
                                                 int_fast64_t x,
                                                 int_fast64_t z,
                                                 int biome,
                                                 double surfaceNoise) {
  if (surfaceNoise > 1.0) {
    topMaterial = STONE;
    groundMaterial = STONE;
  } else {
    topMaterial = GRASS;
    groundMaterial = DIRT;
  }

  GroundGenerator::GenerateTerrainColumn(world, random, x, z, biome, surfaceNoise);
}
