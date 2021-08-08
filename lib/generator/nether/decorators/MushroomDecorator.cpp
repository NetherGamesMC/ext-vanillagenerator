#include <lib/objects/constants/BlockList.h>
#include "MushroomDecorator.h"

void MushroomDecorator::Decorate(ChunkManager &world, Random &random, int_fast32_t chunkX, int_fast32_t chunkZ) {
  int_fast32_t height = world.GetMaxY();

  int_fast32_t sourceX = (chunkX << 4) + random.NextInt(16);
  int_fast32_t sourceZ = (chunkZ << 4) + random.NextInt(16);
  int_fast32_t sourceY = random.NextInt(height);

  for (int_fast32_t i = 0; i < 64; ++i) {
    int_fast32_t x = sourceX + random.NextInt(8) - random.NextInt(8);
    int_fast32_t z = sourceZ + random.NextInt(8) - random.NextInt(8);
    int_fast32_t y = sourceY + random.NextInt(4) - random.NextInt(4);

    int_fast32_t block = world.GetBlockAt(x, y, z).GetId();
    int_fast32_t blockBelow = world.GetBlockAt(x, y - 1, z).GetId();
    if (y < height && block == AIR.GetId()) {
      bool found = false;
      for (MinecraftBlock mcBlock : {NETHERRACK, NETHER_QUARTZ_ORE, SOUL_SAND, GRAVEL}) {
        if (mcBlock.GetId() == blockBelow) {
          found = true;
          break;
        }
      }

      if (!found) {
        world.SetBlockAt(x, y, z, type);
      }
    }
  }
}