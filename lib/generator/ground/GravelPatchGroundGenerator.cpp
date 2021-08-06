//
// Created by User on 3/7/2021.
//

#include "GravelPatchGroundGenerator.h"

typedef GravelPatchGroundGenerator GroundGen;

void GroundGen::GenerateTerrainColumn(ChunkManager &world,
                                      Random &random,
                                      int_fast32_t x,
                                      int_fast32_t z,
                                      int biome,
                                      double surfaceNoise) {
  if (surfaceNoise < -1.0 || surfaceNoise > 2.0) {
    topMaterial = GRAVEL;
    groundMaterial = GRAVEL;
  } else {
    topMaterial = GRASS;
    groundMaterial = DIRT;
  }

  GroundGenerator::GenerateTerrainColumn(world, random, x, z, biome, surfaceNoise);
}
