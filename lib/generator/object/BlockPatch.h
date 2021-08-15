#ifndef EXT_VANILLAGENERATOR_LIB_GENERATOR_OBJECTS_BLOCKPATCH_H_
#define EXT_VANILLAGENERATOR_LIB_GENERATOR_OBJECTS_BLOCKPATCH_H_

#include "TerrainObjects.h"
#include <lib/objects/MinecraftBlock.h>

class BlockPatch {
 public:
  BlockPatch(MinecraftBlock mBlockType, int_fast32_t iHorizontalRadius, int_fast32_t iVerticalRadius, const std::vector<MinecraftBlock> &vOverrides);

  bool Generate(ChunkManager &world, Random &random, int_fast32_t sourceX, int_fast32_t sourceY, int_fast32_t sourceZ);

  std::vector<uint_fast32_t> overrides_;

  MinecraftBlock blockType;
  int_fast32_t horizontalRadius;
  int_fast32_t verticalRadius;
};

#endif //EXT_VANILLAGENERATOR_LIB_GENERATOR_OBJECTS_BLOCKPATCH_H_
