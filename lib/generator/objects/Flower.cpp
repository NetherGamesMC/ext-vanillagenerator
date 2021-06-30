#include <lib/pocketmine/BlockList.h>
#include "Flower.h"

bool Flower::Generate(SimpleChunkManager world,
                      Random &random,
                      int_fast64_t sourceX,
                      int_fast32_t sourceY,
                      int_fast64_t sourceZ) {

  int_fast64_t targetX, targetZ;
  int_fast32_t targetY;

  bool placed = false;
  int height = world.getMaxY();
  for (int i = 0; i < 64; ++i) {
    targetX = sourceX + random.nextInt(8) - random.nextInt(8);
    targetZ = sourceZ + random.nextInt(8) - random.nextInt(8);
    targetY = static_cast<int_fast32_t>(sourceY + random.nextInt(4) - random.nextInt(4));

    MinecraftBlock block = world.getBlockAt(targetX, targetY, targetZ);
    MinecraftBlock blockBelow = world.getBlockAt(targetX, targetY - 1, targetZ);
    if (sourceY < height && block == AIR && blockBelow == GRASS) {
      world.setBlockAt(targetX, targetY, targetZ, this->type_);
      placed = true;
    }
  }

  return placed;
}
