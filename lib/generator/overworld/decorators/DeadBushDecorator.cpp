#include "DeadBushDecorator.h"

using namespace blocks;

void DeadBushDecorator::Decorate(ChunkManager &world, Random &random, int_fast32_t chunkX, int_fast32_t chunkZ) {
  int_fast32_t sourceX, sourceZ;
  int_fast32_t sourceY;

  sourceX = (chunkX << 4) + random.NextInt(16);
  sourceZ = (chunkZ << 4) + random.NextInt(16);
  sourceY = world.GetChunk(chunkX, chunkZ)->GetHighestBlockAt(sourceX & 0x0f, sourceZ & 0x0f);
  sourceY = static_cast<int_fast32_t>(random.NextInt(sourceY << 1));

  while (sourceY > 0 && (world.GetBlockAt(sourceX, sourceY, sourceZ)->GetTypeId() == BlockIds::AIR || world.GetBlockAt(sourceX, sourceY, sourceZ)->GetTypeId() == BlockIds::BIRCH_LEAVES)) {
    --sourceY;
  }

  int_fast32_t y;
  int_fast32_t x, z;
  for (int_fast32_t i = 0; i < 4; ++i) {
    x = sourceX + random.NextInt(8) - random.NextInt(8);
    z = sourceZ + random.NextInt(8) - random.NextInt(8);
    y = static_cast<int_fast32_t>(sourceY + random.NextInt(4) - random.NextInt(4));

    if (world.GetBlockAt(x, y, z)->GetTypeId() == BlockIds::AIR) {
      auto blockBelow = world.GetBlockAt(x, y - 1, z);
      for (auto targets : {BlockIds::SAND, BlockIds::DIRT, BlockIds::HARDENED_CLAY, BlockIds::STAINED_CLAY}) {
        auto block = MCBlock::GetBlockFromStateId(targets);
        if (block == blockBelow) {
          world.SetBlockAt(x, y, z, MCBlock::GetBlockFromStateId(BlockIds::DEAD_BUSH));
          break;
        }
      }
    }
  }

}
