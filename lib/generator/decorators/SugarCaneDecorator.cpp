#include <lib/generator/objects/SugarCane.h>
#include "SugarCaneDecorator.h"

void SugarCaneDecorator::Decorate(ChunkManager &world, Random &random, int_fast32_t chunkX, int_fast32_t chunkZ) {
  int_fast32_t sourceX = (chunkX << 4) + random.NextInt(16);
  int_fast32_t sourceZ = (chunkZ << 4) + random.NextInt(16);
  int_fast16_t maxY = world.GetHighestElevationAt(sourceX & 0x0f, sourceZ & 0x0f);

  if (maxY <= 0) {
    return;
  }

  auto sourceY = static_cast<int_fast16_t>(random.NextInt(maxY << 1));
  for (int j = 0; j < 20; ++j) {
    int_fast32_t x = sourceX + random.NextInt(4) - random.NextInt(4);
    int_fast32_t z = sourceZ + random.NextInt(4) - random.NextInt(4);

    SugarCane::Generate(world, random, x, sourceY, z);
  }
}
