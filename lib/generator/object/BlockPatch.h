#ifndef EXT_NOISELIB_LIB_GENERATOR_OBJECTS_BLOCKPATCH_H_
#define EXT_NOISELIB_LIB_GENERATOR_OBJECTS_BLOCKPATCH_H_

#include "TerrainObjects.h"
#include <lib/objects/MinecraftBlock.h>

class BlockPatch {
 public:
  BlockPatch(MinecraftBlock mBlockType, int iHorizontalRadius, int iVerticalRadius, const std::vector<MinecraftBlock> &vOverrides);

  bool Generate(ChunkManager &world, Random &random, int_fast32_t sourceX, int_fast32_t sourceY, int_fast32_t sourceZ);

  std::vector<uint_fast32_t> overrides_;

  MinecraftBlock blockType;
  int horizontalRadius;
  int verticalRadius;
};

#endif //EXT_NOISELIB_LIB_GENERATOR_OBJECTS_BLOCKPATCH_H_
