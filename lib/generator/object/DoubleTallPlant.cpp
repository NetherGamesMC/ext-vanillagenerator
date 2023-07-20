#include "DoubleTallPlant.h"

bool DoubleTallPlant::Generate(ChunkManager &world, Random &random, int_fast32_t sourceX, int_fast32_t sourceY, int_fast32_t sourceZ) {
  int_fast32_t x, z;
  int_fast32_t y;

  bool placed = false;
  int_fast32_t height = world.GetMaxY();
  for (int_fast32_t i = 0; i < 64; ++i) {
    x = sourceX + random.NextInt(8) - random.NextInt(8);
    z = sourceZ + random.NextInt(8) - random.NextInt(8);
    y = static_cast<int_fast32_t>(sourceY + random.NextInt(4) - random.NextInt(4));

    auto block = world.GetBlockAt(x, y, z)->GetTypeId();
    auto topBlock = world.GetBlockAt(x, static_cast<int_fast32_t>(y + 1), z)->GetTypeId();
    auto belowBlock = world.GetBlockAt(x, y - 1, z)->GetTypeId();
    if (y < height && block == BlockIds::AIR && topBlock == BlockIds::AIR && belowBlock == BlockIds::GRASS) {
      // The first bit is a boolean, indicates that the block is a top.
      world.SetBlockAt(x, y, z, species);
      world.SetBlockAt(x, y + 1, z, MCBlock::GetBlockIdAndMeta(species->GetTypeId(), 1));
      placed = true;
    }
  }

  return placed;
}
