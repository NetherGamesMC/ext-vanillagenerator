#include <lib/pocketmine/BlockList.h>
#include "DoubleTallPlant.h"

bool DoubleTallPlant::generate(SimpleChunkManager world, Random &random, int source_x, int source_y, int source_z) {
  int x, y, z;

  bool placed = false;
  int height = world.getMaxY();
  for (int i = 0; i < 64; ++i) {
    x = source_x + static_cast<int>(random.nextBoundedInt(8) - random.nextBoundedInt(8));
    z = source_z + static_cast<int>(random.nextBoundedInt(8) - random.nextBoundedInt(8));
    y = source_y + static_cast<int>(random.nextBoundedInt(4) - random.nextBoundedInt(4));

    MinecraftBlock block = world.getBlockAt(x, y, z);
    MinecraftBlock top_block = world.getBlockAt(x, y + 1, z);
    if (y < height && block == AIR && top_block == AIR && world.getBlockAt(x, y - 1, z) == GRASS) {
      world.setBlockAt(x, y, z, this->species);
      world.setBlockAt(x, y + 1, z, this->species.makeBlock(0x08, 0b1000));
      placed = true;
    }
  }

  return placed;
}
