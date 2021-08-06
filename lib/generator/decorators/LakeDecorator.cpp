#include <lib/generator/objects/Lake.h>
#include "LakeDecorator.h"

void LakeDecorator::Decorate(ChunkManager &world, Random &random, int_fast64_t chunkX, int_fast64_t chunkZ) {
  if (random.NextInt(rarity) == 0) {
    int_fast64_t sourceX, sourceZ;
    int_fast32_t sourceY;

    sourceX = (chunkX << 4) + random.NextInt(16);
    sourceZ = (chunkZ << 4) + random.NextInt(16);
    sourceY = static_cast<int_fast32_t>(random.NextInt(world.GetMaxY() - baseOffset) + baseOffset);
    if (block.GetId() == 11 && (sourceY >= 64 || random.NextInt(10) > 0)) {
      return;
    }

    while (world.GetBlockAt(sourceX, sourceY, sourceZ).GetId() == 0 && sourceY > 5) {
      --sourceY;
    }

    if (sourceY >= 5) {
      BlockTransaction txn = BlockTransaction(world);
      txn.AddCallback(&VerifyNoWaterBlocks);

      Lake lake = Lake(block, txn);

      if (lake.Generate(world, random, sourceX, sourceY, sourceZ)) {
        txn.ApplyBlockChanges();
      }
    }
  }
}
