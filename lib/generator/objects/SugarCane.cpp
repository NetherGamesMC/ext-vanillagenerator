#include <lib/objects/constants/BlockList.h>
#include "SugarCane.h"

bool SugarCane::Generate(ChunkManager world,
                         Random &random,
                         int_fast64_t sourceX,
                         int_fast32_t sourceY,
                         int_fast64_t sourceZ) {
  if (!(world.getBlockAt(sourceX, sourceY, sourceZ) == AIR)) {
    return false;
  }

  return false;
}
