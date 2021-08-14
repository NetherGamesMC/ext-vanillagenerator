#include <lib/objects/constants/BlockList.h>
#include "DeadBushDecorator.h"

void DeadBushDecorator::Decorate(ChunkManager &world, Random &random, int_fast32_t chunkX, int_fast32_t chunkZ) {
  int_fast32_t sourceX, sourceZ;
  int_fast32_t sourceY;

  sourceX = (chunkX << 4) + random.NextInt(16);
  sourceZ = (chunkZ << 4) + random.NextInt(16);
  sourceY = world.GetChunk(chunkX, chunkZ)->GetHighestBlockAt(sourceX & 0x0f, sourceZ & 0x0f);
  sourceY = static_cast<int_fast32_t>(random.NextInt(sourceY << 1));

  while (sourceY > 0 && (world.GetBlockAt(sourceX, sourceY, sourceZ) == AIR || world.GetBlockAt(sourceX, sourceY, sourceZ).GetId() == 18)) {
    --sourceY;
  }

  int_fast32_t y;
  int_fast32_t x, z;
  for (int_fast32_t i = 0; i < 4; ++i) {
    x = sourceX + random.NextInt(8) - random.NextInt(8);
    z = sourceZ + random.NextInt(8) - random.NextInt(8);
    y = static_cast<int_fast32_t>(sourceY + random.NextInt(4) - random.NextInt(4));

    if (world.GetBlockAt(x, y, z) == AIR) {
      uint_fast32_t blockBelow = world.GetBlockAt(x, y - 1, z).GetId();
      for (MinecraftBlock targets : {SAND, DIRT, HARDENED_CLAY, STAINED_CLAY}) {
        if (targets.GetId() == blockBelow) {
          world.SetBlockAt(x, y, z, DEAD_BUSH);
          break;
        }
      }
    }
  }

}
