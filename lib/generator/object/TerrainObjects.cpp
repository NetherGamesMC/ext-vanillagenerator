#include "TerrainObjects.h"

using namespace blocks;

bool TerrainObjects::KillWeakBlocksAbove(ChunkManager &world, int_fast32_t x, int_fast32_t y, int_fast32_t z) {
  int_fast32_t curY = y + 1;
  bool changed = false;

  while (curY < Chunk::Y_MAX) {
    auto block = world.GetBlockAt(x, curY, z);
    if (!block->IsFlowable()) {
      break;
    }

    world.SetBlockAt(x, curY, z, MCBlock::GetBlockFromStateId(BlockIds::AIR));
    changed = true;
    ++curY;
  }

  return changed;
}