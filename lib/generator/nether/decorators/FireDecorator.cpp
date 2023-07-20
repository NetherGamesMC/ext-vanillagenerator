#include "FireDecorator.h"

void FireDecorator::Decorate(ChunkManager &world, Random &random, int_fast32_t chunkX, int_fast32_t chunkZ) {
  using namespace blocks;

  int_fast32_t amount = 1 + random.NextInt(1 + random.NextInt(10));

  int_fast32_t height = world.GetMaxY();
  int_fast32_t sourceYMargin = 8 * (height >> 7);

  auto airBlock = MCBlock::GetBlockFromStateId(BlockIds::AIR);
  auto netherrackBlock = MCBlock::GetBlockFromStateId(BlockIds::NETHERRACK);
  auto fireBlock = MCBlock::GetBlockFromStateId(BlockIds::FIRE);

  for (int_fast32_t j = 0; j < amount; ++j) {
    int_fast32_t sourceX = (chunkX << 4) + random.NextInt(16);
    int_fast32_t sourceZ = (chunkZ << 4) + random.NextInt(16);
    int_fast32_t sourceY = 4 + random.NextInt(sourceYMargin);

    for (int_fast32_t i = 0; i < 64; ++i) {
      int_fast32_t x = sourceX + random.NextInt(8) - random.NextInt(8);
      int_fast32_t z = sourceZ + random.NextInt(8) - random.NextInt(8);
      int_fast32_t y = sourceY + random.NextInt(4) - random.NextInt(4);

      auto block = world.GetBlockAt(x, y, z);
      auto blockBelow = world.GetBlockAt(x, y - 1, z);
      if (y < height && block == airBlock && blockBelow == netherrackBlock) {
        world.SetBlockAt(x, y, z, fireBlock);
      }
    }
  }
}