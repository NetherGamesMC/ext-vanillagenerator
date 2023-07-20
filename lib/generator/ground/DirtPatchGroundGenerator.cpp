#include "DirtPatchGroundGenerator.h"

typedef DirtPatchGroundGenerator GroundGen;

void GroundGen::GenerateTerrainColumn(ChunkManager &world,
                                      Random &random,
                                      int_fast32_t x,
                                      int_fast32_t z,
                                      int biome,
                                      double surfaceNoise) {
  using namespace blocks;

  if (surfaceNoise > 1.75) {
    topMaterial = MCBlock::GetBlockFromStateId(BlockIds::DIRT);
  } else if (surfaceNoise > -0.5) {
    topMaterial = MCBlock::GetBlockFromStateId(BlockIds::PODZOL);
  } else {
    topMaterial = MCBlock::GetBlockFromStateId(BlockIds::GRASS);
  }
  groundMaterial = MCBlock::GetBlockIdAndMeta(BlockIds::DIRT, 1);

  GroundGenerator::GenerateTerrainColumn(world, random, x, z, biome, surfaceNoise);
}
