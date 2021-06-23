#include <lib/generator/objects/Lake.h>
#include <lib/debug/Debug.h>
#include "LakeDecorator.h"

void LakeDecorator::decorate(SimpleChunkManager &world, Random &random, int chunkX, int chunkZ) {
  if (random.nextBoundedInt(rarity) == 0) {
    int source_x, source_z, source_y;
    source_x = static_cast<int32_t>((chunkX << 4) + random.nextBoundedInt(16));
    source_z = static_cast<int32_t>((chunkZ << 4) + random.nextBoundedInt(16));
    source_y = static_cast<int32_t>(random.nextBoundedInt(world.getMaxY() - baseOffset) + baseOffset);
    if (block.getId() == 11 && (source_y >= 64 || random.nextBoundedInt(10) > 0)) {
      return;
    }

    while (world.getBlockAt(source_x, source_y, source_z).getId() == 0 && source_y > 5) {
      --source_y;
    }

    if (source_y >= 5) {
      Lake(block).generate(world, random, source_x, source_y, source_z)
    }
  }
}
