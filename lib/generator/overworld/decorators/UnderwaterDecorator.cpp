#include <lib/objects/constants/BlockList.h>
#include <lib/generator/object/BlockPatch.h>
#include "UnderwaterDecorator.h"

UnderwaterDecorator::UnderwaterDecorator(MinecraftBlock mType) : type(mType) {}

void UnderwaterDecorator::SetOverridableBlocks(std::vector<MinecraftBlock> fullBlockOverrides) {
  overridable = std::move(fullBlockOverrides);
}

void UnderwaterDecorator::SetRadii(int_fast32_t iHorizontalRadius, int_fast32_t iVerticalRadius) {
  horizontalRadius = iHorizontalRadius;
  verticalRadius = iVerticalRadius;
}

void UnderwaterDecorator::Decorate(ChunkManager &world, Random &random, int_fast32_t chunkX, int_fast32_t chunkZ) {
  int_fast32_t sourceX = (chunkX << 4) + random.NextInt(16);
  int_fast32_t sourceZ = (chunkZ << 4) + random.NextInt(16);
  int_fast32_t sourceY = world.GetHighestElevationAt(sourceX, sourceZ) - 1;

  MinecraftBlock blockId = world.GetBlockAt(sourceX, sourceY - 1, sourceZ);
  while (sourceY > 1 && (blockId == STILL_WATER || blockId == WATER)) {
    --sourceY;
  }

  MinecraftBlock material = world.GetBlockAt(sourceX, sourceY, sourceZ);

  if ((material == STILL_WATER || material == WATER)) {
    (BlockPatch(type, horizontalRadius, verticalRadius, overridable)).Generate(world, random, sourceX, sourceY, sourceZ);
  }
}

