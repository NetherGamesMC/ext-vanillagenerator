#include "BlockPatch.h"

BlockPatch::BlockPatch(MinecraftBlock mBlockType, int iHorizontalRadius, int iVerticalRadius, const std::vector<MinecraftBlock> &vOverrides)
    : blockType(mBlockType), horizontalRadius(iHorizontalRadius), verticalRadius(iVerticalRadius) {
  for (MinecraftBlock block : vOverrides) {
    overrides_.emplace_back(block.GetFullId());
  }
}

bool BlockPatch::Generate(ChunkManager &world, Random &random, int_fast32_t sourceX, int_fast32_t sourceY, int_fast32_t sourceZ) {
  bool success = false;

  int n = random.NextInt(horizontalRadius - 2) + 2;
  int nSquared = n * n;

  for (int x = sourceX - n; x <= sourceX + n; x++) {
    for (int z = sourceZ - n; z <= sourceZ + n; z++) {
      if ((x - sourceX) * (x - sourceX) + (z - sourceZ) * (z - sourceZ) > nSquared) {
        continue;
      }

      for (int y = sourceY - verticalRadius; y <= sourceY + verticalRadius; y++) {

        MinecraftBlock block = world.GetBlockAt(x, y, z);
        if (std::find(overrides_.begin(), overrides_.end(), block.GetFullId()) == overrides_.end()) {
          continue;
        }

        if (TerrainObjects::KillWeakBlocksAbove(world, x, y, z)) {
          continue;
        }

        world.SetBlockAt(x, y, z, blockType);
        success = true;
        break;
      }
    }
  }

  return success;
}
