#include <lib/pocketmine/BlockList.h>
#include "DeadBushDecorator.h"

void DeadBushDecorator::decorate(SimpleChunkManager &world, Random &random, int chunkX, int chunkZ) {
  int sourceX, sourceZ, sourceY;

  sourceX = (chunkX << 4) + random.nextBoundedInt(16);
  sourceZ = (chunkZ << 4) + random.nextBoundedInt(16);
  sourceY = random.nextBoundedInt(world.getChunk(chunkX, chunkZ)
                                      ->getHighestBlockAt(sourceX & 0x0f, sourceZ & 0x0f) << 1);
  while (sourceY > 0
      && (world.getBlockAt(sourceX, sourceY, sourceZ) == AIR
          || world.getBlockAt(sourceX, sourceY, sourceZ).getId() == 18)) {
    --sourceY;
  }

  uint_fast16_t y;
  int_fast64_t x, z;
  for (int i = 0; i < 4; ++i) {
    x = sourceX + random.nextBoundedInt(8) - random.nextBoundedInt(8);
    z = sourceZ + random.nextBoundedInt(8) - random.nextBoundedInt(8);
    y = sourceY + random.nextBoundedInt(4) - random.nextBoundedInt(4);

    if (world.getBlockAt(x, y, z) == AIR) {
      uint_fast16_t block_below = world.getBlockAt(x, y - 1, z).getId();
      for (MinecraftBlock block : {SAND, DIRT, HARDENED_CLAY, STAINED_CLAY}) {
        if (block.getId() == block_below) {
          world.setBlockAt(x, y, z, DEAD_BUSH);
          break;
        }
      }
    }
  }

}
