#include <lib/pocketmine/BlockList.h>
#include "TallGrass.h"

bool TallGrass::generate(SimpleChunkManager world, Random &random, int source_x, int source_y, int source_z) {
  uint_fast16_t currentBlockId;
  do {
    currentBlockId = world.getBlockAt(source_x, source_y, source_z).getId();
    --source_y;
  } while ((currentBlockId == AIR.getId() || currentBlockId == 18) && source_y > 0);
  ++source_y;

  int x, y, z;
  bool succeeded = false;
  int height = world.getMaxY();
  for (int i = 0; i < 128; ++i) {
    x = source_x + random.nextBoundedInt(8) - random.nextBoundedInt(8);
    z = source_z + random.nextBoundedInt(8) - random.nextBoundedInt(8);
    y = source_y + random.nextBoundedInt(4) - random.nextBoundedInt(4);

    MinecraftBlock block_type = world.getBlockAt(x, y, z);
    MinecraftBlock block_type_below = world.getBlockAt(x, y - 1, z);
    if (y < height && block_type == AIR && (block_type_below == GRASS || block_type_below == DIRT)) {
      world.setBlockAt(x, y, z, grassType_);
      succeeded = true;
    }
  }

  return succeeded;
}