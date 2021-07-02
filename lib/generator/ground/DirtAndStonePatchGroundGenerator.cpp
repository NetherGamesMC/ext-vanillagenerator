#include "DirtAndStonePatchGroundGenerator.h"

typedef DirtAndStonePatchGroundGenerator GroundGen;

void GroundGen::GenerateTerrainColumn(SimpleChunkManager &world,
                                      Random &random,
                                      int_fast64_t x,
                                      int_fast64_t z,
                                      int biome,
                                      double surface_noise) {
  if (surface_noise > 1.75) {
    top_material = STONE;
    ground_material = STONE;
  } else if (surface_noise > -0.5) {
    top_material = COARSE_DIRT;
    ground_material = DIRT;
  } else {
    top_material = GRASS;
    ground_material = DIRT;
  }

  GroundGenerator::GenerateTerrainColumn(world, random, x, z, biome, surface_noise);
}
