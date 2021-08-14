#include <lib/objects/constants/BlockList.h>
#include <lib/objects/math/Vector3.h>
#include "SugarCane.h"

bool SugarCane::Generate(ChunkManager &world, Random &random, int_fast32_t sourceX, int_fast16_t sourceY, int_fast32_t sourceZ) {
  if (world.GetBlockAt(sourceX, sourceY, sourceZ) != AIR) {
    return false;
  }

  Vector3 iVec = Vector3(sourceX, sourceY - 1, sourceZ);

  bool bWater = false;
  for (int_fast32_t face : Facing::HORIZONTAL) {
    // needs directly adjacent water block
    auto icVec = iVec.GetSide(face);

    MinecraftBlock blockType = world.GetBlockAt(icVec.GetFloorX(), static_cast<int_fast16_t>(icVec.GetFloorY()), icVec.GetFloorZ());
    if (blockType == STILL_WATER || blockType == WATER) {
      bWater = true;
      break;
    }
  }

  if (!bWater) return false;

  for (int_fast32_t n = 0; n <= random.NextInt(random.NextInt(3) + 1) + 1; ++n) {
    const MinecraftBlock &block = world.GetBlockAt(sourceX, static_cast<int_fast16_t>(sourceY + n - 1), sourceZ);
    const MinecraftBlock &blockAbove = world.GetBlockAt(sourceX, static_cast<int_fast16_t>(sourceY + n), sourceZ);
    if ((block == SUGARCANE || block == GRASS || block == SAND || block == COARSE_DIRT)) {
      MinecraftBlock caneBlock = world.GetBlockAt(sourceX, static_cast<int_fast16_t>(sourceY + n), sourceZ);
      if (caneBlock != AIR && world.GetBlockAt(sourceX, static_cast<int_fast16_t>(sourceY + n + 1), sourceZ) != AIR) {
        return n > 0;
      }

      world.SetBlockAt(sourceX, static_cast<int_fast16_t>(sourceY + n), sourceZ, SUGARCANE);
    }
  }

  return false;
}
