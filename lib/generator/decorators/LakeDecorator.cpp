#include <lib/generator/objects/Lake.h>
#include "LakeDecorator.h"

void LakeDecorator::Decorate(ChunkManager &world, Random &random, int_fast64_t chunkX, int_fast64_t chunkZ) {
  if (random.nextInt(rarity) == 0) {
    int_fast64_t source_x, source_z;
    int_fast32_t source_y;

    source_x = (chunkX << 4) + random.nextInt(16);
    source_z = (chunkZ << 4) + random.nextInt(16);
    source_y = static_cast<int_fast32_t>(random.nextInt(world.getMaxY() - baseOffset) + baseOffset);
    if (block.getId() == 11 && (source_y >= 64 || random.nextInt(10) > 0)) {
      return;
    }

    while (world.getBlockAt(source_x, source_y, source_z).getId() == 0 && source_y > 5) {
      --source_y;
    }

    if (source_y >= 5) {
      BlockTransaction txn = BlockTransaction(world);
      txn.AddCallback(&VerifyNoWaterBlocks);

      Lake lake = Lake(block, txn);

      if (lake.Generate(world, random, source_x, source_y, source_z)) {
        txn.ApplyBlockChanges();
      }
    }
  }
}
