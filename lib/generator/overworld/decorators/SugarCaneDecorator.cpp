#include <lib/generator/object/SugarCane.h>
#include "SugarCaneDecorator.h"

void SugarCaneDecorator::Decorate(ChunkManager &world, Random &random, int_fast32_t chunkX, int_fast32_t chunkZ) {
  int_fast32_t sourceX = (chunkX << 4) + static_cast<int_fast32_t>(random.NextInt(16));
  int_fast32_t sourceZ = (chunkZ << 4) + static_cast<int_fast32_t>(random.NextInt(16));
  int_fast32_t maxY = world.GetChunk(chunkX, chunkZ)->GetHighestBlockAt(sourceX & 0x0f, sourceZ & 0x0f);

  if (maxY <= 0) {
    return;
  }

  auto sourceY = static_cast<int_fast32_t>(random.NextInt(maxY << 1));
  for (int_fast32_t j = 0; j < 20; ++j) {
    int_fast32_t x = sourceX + static_cast<int_fast32_t>(random.NextInt(4) - random.NextInt(4));
    int_fast32_t z = sourceZ + static_cast<int_fast32_t>(random.NextInt(4) - random.NextInt(4));

    SugarCane::Generate(world, random, x, sourceY, z);
  }
}
