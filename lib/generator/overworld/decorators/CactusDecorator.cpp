#include <lib/generator/object/Cactus.h>
#include "CactusDecorator.h"

void CactusDecorator::Decorate(ChunkManager &world, Random &random, int_fast32_t chunkX, int_fast32_t chunkZ) {
  int_fast32_t sourceX, sourceZ, x, z;
  int_fast16_t sourceY;

  sourceX = (chunkX << 4);
  sourceZ = (chunkZ << 4);
  x = random.NextInt(16);
  z = random.NextInt(16);

  sourceY = static_cast<int_fast16_t>(random.NextInt(world.GetChunk(chunkX, chunkZ)->GetHighestBlockAt(x, z) << 1));

  for (int_fast32_t l = 0; l < 10; ++l) {
    int_fast32_t i = sourceX + random.NextInt(8) - random.NextInt(8);
    int_fast32_t k = sourceZ + random.NextInt(8) - random.NextInt(8);
    auto j = static_cast<int_fast16_t>(sourceY + random.NextInt(4) - random.NextInt(4));

    Cactus::Generate(world, random, x + i, j, z + k);
  }
}
