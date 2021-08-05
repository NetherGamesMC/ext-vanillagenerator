#include <lib/objects/constants/BlockList.h>
#include "DeadBushDecorator.h"

void DeadBushDecorator::Decorate(ChunkManager &world, Random &random, int_fast64_t chunkX, int_fast64_t chunkZ) {
  int_fast64_t sourceX, sourceZ;
  int_fast32_t sourceY;

  sourceX = (chunkX << 4) + random.NextInt(16);
  sourceZ = (chunkZ << 4) + random.NextInt(16);
  sourceY = world.GetChunk(chunkX, chunkZ)->GetHighestBlockAt(sourceX & 0x0f, sourceZ & 0x0f);
  sourceY = static_cast<int_fast32_t>(random.NextInt(sourceY << 1));

  auto block = world.GetBlockAt(sourceX, sourceY, sourceZ);
  while (sourceY > 0 && (block == AIR || block.GetId() == 18)) {
    --sourceY;
  }

  int_fast32_t y;
  int_fast64_t x, z;
  for (int i = 0; i < 4; ++i) {
    x = sourceX + random.NextInt(8) - random.NextInt(8);
    z = sourceZ + random.NextInt(8) - random.NextInt(8);
    y = static_cast<int_fast32_t>(sourceY + random.NextInt(4) - random.NextInt(4));

    if (world.GetBlockAt(x, y, z) == AIR) {
      uint_fast32_t block_below = world.GetBlockAt(x, y - 1, z).GetId();
      for (MinecraftBlock targets : {SAND, DIRT, HARDENED_CLAY, STAINED_CLAY}) {
        if (targets.GetId() == block_below) {
          world.SetBlockAt(x, y, z, DEAD_BUSH);
          break;
        }
      }
    }
  }

}
