#include "MelonDecorator.h"

using namespace blocks;

void MelonDecorator::Decorate(ChunkManager &world, Random &random, int_fast32_t chunkX, int_fast32_t chunkZ) {
  int_fast64_t sourceX = (chunkX << 4) + random.NextInt(16);
  int_fast64_t sourceZ = (chunkZ << 4) + random.NextInt(16);
  int_fast64_t sourceY = random.NextInt(64 << 1);

  for (int_fast32_t i = 0; i < 64; i++) {
    int_fast32_t x = sourceX + random.NextInt(8) - random.NextInt(8);
    int_fast32_t z = sourceZ + random.NextInt(8) - random.NextInt(8);
    int_fast32_t y = sourceY + random.NextInt(4) - random.NextInt(4);

    if (world.GetBlockAt(x, y, z)->GetTypeId() == BlockIds::AIR && world.GetBlockAt(x, y - 1, z)->GetTypeId() == BlockIds::GRASS) {
      world.SetBlockAt(x, y, z, MCBlock::GetBlockFromStateId(BlockIds::MELON));
    }
  }
}
