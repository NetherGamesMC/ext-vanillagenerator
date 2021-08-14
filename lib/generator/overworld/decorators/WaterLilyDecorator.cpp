#include <lib/objects/constants/BlockList.h>
#include <lib/objects/constants/Logic.h>
#include "WaterLilyDecorator.h"

void WaterLilyDecorator::Decorate(ChunkManager &world, Random &random, int_fast32_t chunkX, int_fast32_t chunkZ) {
  int_fast32_t sourceX = (chunkX << 4) + static_cast<int_fast32_t>(random.NextInt(16));
  int_fast32_t sourceZ = (chunkZ << 4) + static_cast<int_fast32_t>(random.NextInt(16));
  auto sourceY = static_cast<int_fast32_t>(random.NextInt(world.GetHighestElevationAt(sourceX, sourceZ) << 1));
  while (world.GetBlockAt(sourceX, sourceY - 1, sourceZ) == AIR && sourceY > 0) {
    --sourceY;
  }

  for (int_fast32_t j = 0; j < 10; ++j) {
    int_fast32_t x = sourceX + static_cast<int_fast32_t>(random.NextInt(8) - random.NextInt(8));
    int_fast32_t z = sourceZ + static_cast<int_fast32_t>(random.NextInt(8) - random.NextInt(8));
    int_fast32_t y = sourceY + static_cast<int_fast32_t>(random.NextInt(4) - random.NextInt(4));

    if (y >= 0 && y < Y_MAX && world.GetBlockAt(x, y, z) == AIR && world.GetBlockAt(x, y - 1, z) == STILL_WATER) {
      world.SetBlockAt(x, y, z, LILY_PAD);
    }
  }
}
