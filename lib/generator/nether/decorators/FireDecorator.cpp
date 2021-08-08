#include <lib/objects/constants/BlockList.h>
#include "FireDecorator.h"

void FireDecorator::Decorate(ChunkManager &world, Random &random, int_fast32_t chunkX, int_fast32_t chunkZ) {
  int_fast32_t amount = 1 + random.NextInt(1 + random.NextInt(10));

  int_fast32_t height = world.GetMaxY();
  int_fast32_t sourceYMargin = 8 * (height >> 7);

  for (int_fast32_t j = 0; j < amount; ++j) {
    int_fast32_t sourceX = (chunkX << 4) + random.NextInt(16);
    int_fast32_t sourceZ = (chunkZ << 4) + random.NextInt(16);
    int_fast32_t sourceY = 4 + random.NextInt(sourceYMargin);

    for (int_fast32_t i = 0; i < 64; ++i) {
      int_fast32_t x = sourceX + random.NextInt(8) - random.NextInt(8);
      int_fast32_t z = sourceZ + random.NextInt(8) - random.NextInt(8);
      int_fast32_t y = sourceY + random.NextInt(4) - random.NextInt(4);

      uint_fast32_t block = world.GetBlockAt(x, y, z).GetId();
      uint_fast32_t blockBelow = world.GetBlockAt(x, y - 1, z).GetId();
      if (y < height && block == AIR.GetId() && blockBelow == NETHERRACK.GetId()) {
        world.SetBlockAt(x, y, z, FIRE);
      }
    }
  }
}