#include <lib/objects/constants/BlockList.h>
#include "DoubleTallPlant.h"

bool DoubleTallPlant::Generate(ChunkManager &world,
                               Random &random,
                               int_fast32_t sourceX,
                               int_fast32_t sourceY,
                               int_fast32_t sourceZ) {

  int_fast32_t x, z;
  int_fast32_t y;

  bool placed = false;
  int_fast32_t height = world.GetMaxY();
  for (int_fast32_t i = 0; i < 64; ++i) {
    x = sourceX + random.NextInt(8) - random.NextInt(8);
    z = sourceZ + random.NextInt(8) - random.NextInt(8);
    y = static_cast<int_fast32_t>(sourceY + random.NextInt(4) - random.NextInt(4));

    MinecraftBlock block = world.GetBlockAt(x, y, z);
    MinecraftBlock topBlock = world.GetBlockAt(x, static_cast<int_fast32_t>(y + 1), z);
    if (y < height && block == AIR && topBlock == AIR && world.GetBlockAt(x, y - 1, z) == GRASS) {
      world.SetBlockAt(x, y, z, species_);
      world.SetBlockAt(x, y + 1, z, species_.MakeBlock(0x08, 0b1000));
      placed = true;
    }
  }

  return placed;
}
