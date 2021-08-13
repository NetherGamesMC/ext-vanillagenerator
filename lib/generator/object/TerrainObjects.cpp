#include <lib/objects/constants/Logic.h>
#include <lib/objects/constants/BlockList.h>
#include "TerrainObjects.h"

bool TerrainObjects::KillWeakBlocksAbove(ChunkManager &world, int_fast32_t x, int_fast32_t y, int_fast32_t z) {
  int_fast32_t curY = y + 1;
  bool changed = false;

  while (curY < Y_MAX) {
    auto block = world.GetBlockAt(x, curY, z);
    if (!IS_FLOWABLE(block.GetId())) {
      break;
    }

    world.SetBlockAt(x, curY, z, AIR);
    changed = true;
    ++curY;
  }

  return changed;
}