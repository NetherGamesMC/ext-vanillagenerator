#ifndef EXT_VANILLAGENERATOR_LIB_GENERATOR_GROUND_GROUNDGENERATOR_H_
#define EXT_VANILLAGENERATOR_LIB_GENERATOR_GROUND_GROUNDGENERATOR_H_

#include <lib/chunk/ChunkManager.h>
#include <lib/objects/random/Random.h>

using namespace blocks;

class GroundGenerator {
 public:
  virtual void GenerateTerrainColumn(ChunkManager &world, Random &random, int_fast32_t x, int_fast32_t z, int biome, double surfaceNoise);
 protected:

  const MCBlock *topMaterial = MCBlock::GetBlockFromStateId(BlockIds::GRASS);
  const MCBlock *groundMaterial = MCBlock::GetBlockIdAndMeta(BlockIds::DIRT, 1);
};

#endif // EXT_VANILLAGENERATOR_LIB_GENERATOR_GROUND_GROUNDGENERATOR_H_
