#include <lib/pocketmine/BlockList.h>
#include "DeadBushDecorator.h"

void DeadBushDecorator::Decorate(SimpleChunkManager &world, Random &random, int_fast64_t chunkX, int_fast64_t chunkZ) {
  int_fast64_t sourceX, sourceZ;
  int_fast32_t sourceY;

  sourceX = (chunkX << 4) + random.nextInt(16);
  sourceZ = (chunkZ << 4) + random.nextInt(16);
  sourceY = world.getChunk(chunkX, chunkZ)->getHighestBlockAt(sourceX & 0x0f, sourceZ & 0x0f);
  sourceY = static_cast<int_fast32_t>(random.nextInt(sourceY << 1));

  auto block = world.getBlockAt(sourceX, sourceY, sourceZ);
  while (sourceY > 0 && (block == AIR || block.getId() == 18)) {
    --sourceY;
  }

  int_fast32_t y;
  int_fast64_t x, z;
  for (int i = 0; i < 4; ++i) {
    x = sourceX + random.nextInt(8) - random.nextInt(8);
    z = sourceZ + random.nextInt(8) - random.nextInt(8);
    y = static_cast<int_fast32_t>(sourceY + random.nextInt(4) - random.nextInt(4));

    if (world.getBlockAt(x, y, z) == AIR) {
      uint_fast32_t block_below = world.getBlockAt(x, y - 1, z).getId();
      for (MinecraftBlock targets : {SAND, DIRT, HARDENED_CLAY, STAINED_CLAY}) {
        if (targets.getId() == block_below) {
          world.setBlockAt(x, y, z, DEAD_BUSH);
          break;
        }
      }
    }
  }

}
