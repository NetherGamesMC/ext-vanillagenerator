#include <lib/pocketmine/BlockList.h>
#include "DoubleTallPlant.h"

bool DoubleTallPlant::Generate(SimpleChunkManager world,
                               Random &random,
                               int_fast64_t sourceX,
                               int_fast32_t sourceY,
                               int_fast64_t sourceZ) {

  int_fast64_t x, z;
  int_fast32_t y;

  bool placed = false;
  int height = world.getMaxY();
  for (int i = 0; i < 64; ++i) {
    x = sourceX + random.nextInt(8) - random.nextInt(8);
    z = sourceZ + random.nextInt(8) - random.nextInt(8);
    y = static_cast<int_fast32_t>(sourceY + random.nextInt(4) - random.nextInt(4));

    MinecraftBlock block = world.getBlockAt(x, y, z);
    MinecraftBlock top_block = world.getBlockAt(x, static_cast<int_fast32_t>(y + 1), z);
    if (y < height && block == AIR && top_block == AIR && world.getBlockAt(x, y - 1, z) == GRASS) {
      world.setBlockAt(x, y, z, species_);
      world.setBlockAt(x, y + 1, z, species_.makeBlock(0x08, 0b1000));
      placed = true;
    }
  }

  return placed;
}
