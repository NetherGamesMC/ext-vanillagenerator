#include "StonePatchGroundGenerator.h"

void StonePatchGroundGenerator::GenerateTerrainColumn(ChunkManager &world,
                                                 Random &random,
                                                 int_fast32_t x,
                                                 int_fast32_t z,
                                                 int biome,
                                                 double surfaceNoise) {
  using namespace blocks;

  if (surfaceNoise > 1.0) {
    topMaterial = MCBlock::GetBlockFromStateId(BlockIds::STONE);
    groundMaterial = MCBlock::GetBlockFromStateId(BlockIds::STONE);
  } else {
    topMaterial = MCBlock::GetBlockFromStateId(BlockIds::GRASS);
    groundMaterial = MCBlock::GetBlockIdAndMeta(BlockIds::DIRT, 1);
  }

  GroundGenerator::GenerateTerrainColumn(world, random, x, z, biome, surfaceNoise);
}
