#include <lib/objects/constants/BlockList.h>
#include <lib/objects/math/Vector3.h>
#include "SugarCane.h"

bool SugarCane::Generate(ChunkManager &world, Random &random, int_fast32_t sourceX, int_fast16_t sourceY, int_fast32_t sourceZ) {
  if (!(world.GetBlockAt(sourceX, sourceY, sourceZ) == AIR)) {
    return false;
  }

  Vector3 iVec = {sourceX, sourceY, sourceZ};

  bool bWater = false;
  for (int face : Facing::HORIZONTAL) {
    // needs directly adjacent water block
    auto icVec = getSide(iVec, face, 1);

    MinecraftBlock blockType = world.GetBlockAt(icVec.x, static_cast<int_fast16_t>(icVec.y), icVec.z);
    if (blockType == STILL_WATER || blockType == WATER) {
      bWater = true;
      break;
    }
  }

  if (!bWater) return false;

  for (int n = 0; n <= random.NextInt(random.NextInt(3) + 1) + 1; ++n) {
    MinecraftBlock block = world.GetBlockAt(sourceX, static_cast<int_fast16_t>(sourceY + n - 1), sourceZ);
    if (block == SUGARCANE || block == GRASS || block == SAND || block == COARSE_DIRT) {
      MinecraftBlock cane_block = world.GetBlockAt(sourceX, static_cast<int_fast16_t>(sourceY + n), sourceZ);
      if (!(cane_block == AIR) && !(world.GetBlockAt(sourceX, static_cast<int_fast16_t>(sourceY + n + 1), sourceZ) == AIR)) {
        return n > 0;
      }

      world.SetBlockAt(sourceX, static_cast<int_fast16_t>(sourceY + n), sourceZ, SUGARCANE);
    }
  }

  return false;
}
