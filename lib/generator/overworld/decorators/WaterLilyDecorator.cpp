#include "WaterLilyDecorator.h"

using namespace blocks;

void WaterLilyDecorator::Decorate(ChunkManager &world, Random &random, int_fast32_t chunkX, int_fast32_t chunkZ) {
  int_fast32_t sourceX = (chunkX << 4) + static_cast<int_fast32_t>(random.NextInt(16));
  int_fast32_t sourceZ = (chunkZ << 4) + static_cast<int_fast32_t>(random.NextInt(16));
  auto sourceY = static_cast<int_fast32_t>(random.NextInt(world.GetHighestElevationAt(sourceX, sourceZ) << 1));
  while (world.GetBlockAt(sourceX, sourceY - 1, sourceZ)->GetTypeId() == BlockIds::AIR && sourceY > 0) {
    --sourceY;
  }

  for (int_fast32_t j = 0; j < 10; ++j) {
    int_fast32_t x = sourceX + static_cast<int_fast32_t>(random.NextInt(8) - random.NextInt(8));
    int_fast32_t z = sourceZ + static_cast<int_fast32_t>(random.NextInt(8) - random.NextInt(8));
    int_fast32_t y = sourceY + static_cast<int_fast32_t>(random.NextInt(4) - random.NextInt(4));

    if (y >= 0 && y < Chunk::Y_MAX && world.GetBlockAt(x, y, z)->GetTypeId() == BlockIds::AIR && world.GetBlockAt(x, y - 1, z)->GetTypeId() == BlockIds::WATER) {
      world.SetBlockAt(x, y, z, MCBlock::GetBlockFromStateId(BlockIds::LILY_PAD));
    }
  }
}
