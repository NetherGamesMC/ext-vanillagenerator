#include <lib/generator/objects/Lake.h>
#include "LakeDecorator.h"

void LakeDecorator::Decorate(ChunkManager &world, Random &random, int_fast32_t chunkX, int_fast32_t chunkZ) {
  if (random.NextInt(rarity) == 0) {
    int_fast32_t source_x, source_z;
    int_fast32_t source_y;

    source_x = (chunkX << 4) + random.NextInt(16);
    source_z = (chunkZ << 4) + random.NextInt(16);
    source_y = random.NextInt(world.GetMaxY() - baseOffset) + baseOffset;
    if (block.GetId() == 11 && (source_y >= 64 || random.NextInt(10) > 0)) {
      return;
    }

    while (world.GetBlockAt(source_x, source_y, source_z).GetId() == 0 && source_y > 5) {
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
