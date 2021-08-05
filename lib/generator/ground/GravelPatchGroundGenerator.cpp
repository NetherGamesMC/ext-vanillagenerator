//
// Created by User on 3/7/2021.
//

#include "GravelPatchGroundGenerator.h"

typedef GravelPatchGroundGenerator GroundGen;

void GroundGen::GenerateTerrainColumn(ChunkManager &world,
                                      Random &random,
                                      int_fast64_t x,
                                      int_fast64_t z,
                                      int biome,
                                      double surface_noise) {
  if (surface_noise < -1.0 || surface_noise > 2.0) {
    top_material = GRAVEL;
    ground_material = GRAVEL;
  } else {
    top_material = GRASS;
    ground_material = DIRT;
  }

  GroundGenerator::GenerateTerrainColumn(world, random, x, z, biome, surface_noise);
}
