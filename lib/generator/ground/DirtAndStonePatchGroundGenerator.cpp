#include "DirtAndStonePatchGroundGenerator.h"

void DirtAndStonePatchGroundGenerator::GenerateTerrainColumn(ChunkManager &world, Random &random, int_fast32_t x, int_fast32_t z, int biome, double surfaceNoise) {
  if (surfaceNoise > 1.75) {
    topMaterial = MCBlock::GetBlockFromStateId(BlockIds::STONE);
    groundMaterial = MCBlock::GetBlockFromStateId(BlockIds::STONE);
  } else if (surfaceNoise > -0.5) {
    // For coarse dirt, 2-bits were used, 0 -> coarse, 1 -> normal
    topMaterial = MCBlock::GetBlockFromStateId(BlockIds::DIRT);
    groundMaterial = MCBlock::GetBlockIdAndMeta(BlockIds::DIRT, 1);
  } else {
    topMaterial = MCBlock::GetBlockFromStateId(BlockIds::GRASS);
    groundMaterial = MCBlock::GetBlockIdAndMeta(BlockIds::DIRT, 1);
  }

  GroundGenerator::GenerateTerrainColumn(world, random, x, z, biome, surfaceNoise);
}
