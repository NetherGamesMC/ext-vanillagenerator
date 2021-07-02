#include "StonePatchGroundGenerator.h"

void StonePatchGroundGenerator::GenerateTerrainColumn(SimpleChunkManager &world,
                                                 Random &random,
                                                 int_fast64_t x,
                                                 int_fast64_t z,
                                                 int biome,
                                                 double surface_noise) {
  if (surface_noise > 1.0) {
    top_material = STONE;
    ground_material = STONE;
  } else {
    top_material = GRASS;
    ground_material = DIRT;
  }

  GroundGenerator::GenerateTerrainColumn(world, random, x, z, biome, surface_noise);
}
