#include <lib/generator/object/Lake.h>
#include <lib/objects/constants/BlockList.h>
#include "LakeDecorator.h"

void LakeDecorator::Decorate(ChunkManager &world, Random &random, int_fast32_t chunkX, int_fast32_t chunkZ) {
  if (static_cast<int_fast32_t>(rarity) == 0) {
    int_fast32_t sourceX, sourceZ;
    int_fast32_t sourceY;

    sourceX = (chunkX << 4) + static_cast<int_fast32_t>(random.NextInt(16));
    sourceZ = (chunkZ << 4) + static_cast<int_fast32_t>(random.NextInt(16));
    sourceY = static_cast<int_fast32_t>(random.NextInt(world.GetMaxY() - baseOffset)) + baseOffset;
    if ((block == LAVA || block == STILL_LAVA) && (sourceY >= 64 || random.NextInt(10) > 0)) {
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
