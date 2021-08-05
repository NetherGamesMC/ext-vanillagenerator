#include <lib/objects/constants/Logic.h>
#include <lib/objects/constants/BlockList.h>
#include "TerrainObjects.h"

bool TerrainObjects::killWeakBlocksAbove(ChunkManager &world, int_fast64_t x, int_fast32_t y, int_fast64_t z) {
  int_fast32_t cur_y = y + 1;
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