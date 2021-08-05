#include "SugarCaneDecorator.h"

void SugarCaneDecorator::Decorate(ChunkManager &world, Random &random, int_fast64_t chunkX, int_fast64_t chunkZ) {
  int_fast64_t source_x = (chunkX << 4) + random.NextInt(16);
  int_fast64_t source_z = (chunkZ << 4) + random.NextInt(16);
  int_fast16_t max_y = world.GetHighestElevationAt(source_x & 0x0f, source_z & 0x0f);
  if (max_y <= 0) {
    return;
  }
  int_fast64_t source_y = random.NextInt(max_y << 1);
  for (int j = 0; j < 20; ++j) {
    int_fast64_t x = source_x + random.NextInt(4) - random.NextInt(4);
    int_fast64_t z = source_z + random.NextInt(4) - random.NextInt(4);
    //(new SugarCane()).generate(world, random, x, source_y, z);
  }
}
