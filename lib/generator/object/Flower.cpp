#include <lib/objects/constants/BlockList.h>
#include "Flower.h"

bool Flower::Generate(ChunkManager &world,
                      Random &random,
                      int_fast32_t sourceX,
                      int_fast32_t sourceY,
                      int_fast32_t sourceZ) {

  int_fast32_t targetX, targetZ;
  int_fast32_t targetY;

  bool placed = false;
  int_fast32_t height = world.GetMaxY();
  for (int_fast32_t i = 0; i < 64; ++i) {
    targetX = sourceX + random.NextInt(8) - random.NextInt(8);
    targetZ = sourceZ + random.NextInt(8) - random.NextInt(8);
    targetY = static_cast<int_fast32_t>(sourceY + random.NextInt(4) - random.NextInt(4));

    MinecraftBlock block = world.GetBlockAt(targetX, targetY, targetZ);
    MinecraftBlock blockBelow = world.GetBlockAt(targetX, targetY - 1, targetZ);
    if (sourceY < height && block == AIR && blockBelow == GRASS) {
      world.SetBlockAt(targetX, targetY, targetZ, this->type_);
      placed = true;
    }
  }

  return placed;
}
