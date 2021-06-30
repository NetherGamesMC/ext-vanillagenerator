#include <lib/pocketmine/BlockList.h>
#include "TallGrass.h"

bool TallGrass::Generate(SimpleChunkManager world,
                         Random &random,
                         int_fast64_t sourceX,
                         int_fast32_t sourceY,
                         int_fast64_t sourceZ) {

  uint_fast32_t currentBlockId;
  do {
    currentBlockId = world.getBlockAt(sourceX, sourceY, sourceZ).getId();
    --sourceY;
  } while ((currentBlockId == AIR.getId() || currentBlockId == 18) && sourceY > 0);
  ++sourceY;

  int_fast64_t x, z;
  int_fast32_t y;

  bool succeeded = false;
  int height = world.getMaxY();
  for (int i = 0; i < 128; ++i) {
    x = sourceX + random.nextInt(8) - random.nextInt(8);
    z = sourceZ + random.nextInt(8) - random.nextInt(8);
    y = static_cast<int_fast32_t>(sourceY + random.nextInt(4) - random.nextInt(4));

    MinecraftBlock block_type = world.getBlockAt(x, y, z);
    MinecraftBlock block_type_below = world.getBlockAt(x, y - 1, z);
    if (y < height && block_type == AIR && (block_type_below == GRASS || block_type_below == DIRT)) {
      world.setBlockAt(x, y, z, grassType_);
      succeeded = true;
    }
  }

  return succeeded;
}