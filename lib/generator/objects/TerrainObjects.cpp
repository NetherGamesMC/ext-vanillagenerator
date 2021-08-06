#include <lib/objects/constants/Logic.h>
#include <lib/objects/constants/BlockList.h>
#include "TerrainObjects.h"

bool TerrainObjects::KillWeakBlocksAbove(ChunkManager &world, int_fast32_t x, int_fast32_t y, int_fast32_t z) {
  int_fast32_t cur_y = y + 1;
  bool changed = false;

  while (cur_y < Y_MAX) {
    auto block = world.GetBlockAt(x, cur_y, z);
    if (!IS_FLOWABLE(block.GetId())) {
      break;
    }

    world.SetBlockAt(x, cur_y, z, AIR);
    changed = true;
    ++cur_y;
  }

  return changed;
}