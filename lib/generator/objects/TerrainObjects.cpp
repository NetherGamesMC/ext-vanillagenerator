#include <lib/pocketmine/Logic.h>
#include <lib/pocketmine/BlockList.h>
#include "TerrainObjects.h"

bool TerrainObjects::killWeakBlocksAbove(SimpleChunkManager &world, int x, int y, int z) {
  int cur_y = y + 1;
  bool changed = false;

  while (cur_y < Y_MAX) {
    auto block = world.getBlockAt(x, cur_y, z);
    if (!IS_FLOWABLE(block.getId())) {
      break;
    }

    world.setBlockAt(x, cur_y, z, AIR);
    changed = true;
    ++cur_y;
  }

  return changed;
}
