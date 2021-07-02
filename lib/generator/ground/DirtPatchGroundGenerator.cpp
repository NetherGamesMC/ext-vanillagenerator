#include "DirtPatchGroundGenerator.h"

typedef DirtPatchGroundGenerator GroundGen;

void GroundGen::GenerateTerrainColumn(SimpleChunkManager &world,
                                      Random &random,
                                      int_fast64_t x,
                                      int_fast64_t z,
                                      int biome,
                                      double surface_noise) {
  if (surface_noise > 1.75) {
    top_material = COARSE_DIRT;
  } else if (surface_noise > -0.5) {
    top_material = PODZOL;
  } else {
    top_material = GRASS;
  }
  ground_material = DIRT;

  GroundGenerator::GenerateTerrainColumn(world, random, x, z, biome, surface_noise);
}
