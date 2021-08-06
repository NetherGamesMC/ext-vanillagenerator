#include "SugarCaneDecorator.h"

void SugarCaneDecorator::Decorate(ChunkManager &world, Random &random, int_fast64_t chunkX, int_fast64_t chunkZ) {
  int_fast64_t sourceX = (chunkX << 4) + random.NextInt(16);
  int_fast64_t sourceZ = (chunkZ << 4) + random.NextInt(16);
  int_fast16_t maxY = world.GetHighestElevationAt(sourceX & 0x0f, sourceZ & 0x0f);
  if (maxY <= 0) {
    return;
  }
  int_fast64_t source_y = random.NextInt(maxY << 1);
  for (int j = 0; j < 20; ++j) {
    int_fast64_t x = sourceX + random.NextInt(4) - random.NextInt(4);
    int_fast64_t z = sourceZ + random.NextInt(4) - random.NextInt(4);
    //(new SugarCane()).generate(world, random, x, source_y, z);
  }
}
