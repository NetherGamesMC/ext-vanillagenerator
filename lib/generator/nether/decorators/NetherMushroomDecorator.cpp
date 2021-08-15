#include <lib/objects/constants/BlockList.h>
#include "NetherMushroomDecorator.h"

void NetherMushroomDecorator::Decorate(ChunkManager &world, Random &random, int_fast32_t chunkX, int_fast32_t chunkZ) {
  int_fast32_t height = world.GetMaxY();

  int_fast32_t sourceX = (chunkX << 4) + static_cast<int_fast32_t>(random.NextInt(16));
  int_fast32_t sourceZ = (chunkZ << 4) + static_cast<int_fast32_t>(random.NextInt(16));
  auto sourceY = static_cast<int_fast16_t>(random.NextInt(height));

  for (int_fast32_t i = 0; i < 64; ++i) {
    int_fast32_t x = sourceX + static_cast<int_fast32_t>(random.NextInt(8) - random.NextInt(8));
    int_fast32_t z = sourceZ + static_cast<int_fast32_t>(random.NextInt(8) - random.NextInt(8));
    auto y = static_cast<int_fast16_t>(sourceY + random.NextInt(4) - random.NextInt(4));

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

      if (found) {
        world.SetBlockAt(x, y, z, type);
      }
    }
  }
}