#include "StonePatchGroundGenerator.h"

void StonePatchGroundGenerator::GenerateTerrainColumn(ChunkManager &world,
                                                 Random &random,
                                                 int_fast32_t x,
                                                 int_fast32_t z,
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
