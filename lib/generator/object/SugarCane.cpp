#include <lib/objects/math/Vector3.h>
#include "SugarCane.h"

using namespace blocks;

bool SugarCane::Generate(ChunkManager &world, Random &random, int_fast32_t sourceX, int_fast16_t sourceY, int_fast32_t sourceZ) {
  if (world.GetBlockAt(sourceX, sourceY, sourceZ)->GetTypeId() != BlockIds::AIR) {
    return false;
  }

  Vector3 iVec = Vector3(sourceX, sourceY - 1, sourceZ);

  bool bWater = false;
  for (int_fast32_t face : Facing::HORIZONTAL) {
    // needs directly adjacent water block
    auto icVec = iVec.GetSide(face);

    auto blockType = world.GetBlockAt(icVec.GetFloorX(), static_cast<int_fast16_t>(icVec.GetFloorY()), icVec.GetFloorZ())->GetTypeId();
    if (blockType == BlockIds::WATER) {
      bWater = true;
      break;
    }
  }

  if (!bWater) return false;

  for (int_fast32_t n = 0; n <= random.NextInt(random.NextInt(3) + 1) + 1; ++n) {
    auto block = world.GetBlockAt(sourceX, static_cast<int_fast16_t>(sourceY + n - 1), sourceZ)->GetTypeId();
    auto blockAbove = world.GetBlockAt(sourceX, static_cast<int_fast16_t>(sourceY + n), sourceZ)->GetTypeId();
    if ((block == BlockIds::SUGARCANE || block == BlockIds::GRASS || block == BlockIds::SAND || block == BlockIds::DIRT)) {
      if (blockAbove != BlockIds::AIR && world.GetBlockAt(sourceX, static_cast<int_fast16_t>(sourceY + n + 1), sourceZ)->GetTypeId() != BlockIds::AIR) {
        return n > 0;
      }

      world.SetBlockAt(sourceX, static_cast<int_fast16_t>(sourceY + n), sourceZ, MCBlock::GetBlockFromStateId(BlockIds::SUGARCANE));
    }
  }

  return false;
}
