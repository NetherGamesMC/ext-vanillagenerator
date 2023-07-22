#include "TallRedwoodTree.h"

void TallRedwoodTree::Initialize(Random &random, BlockTransaction &txn) {
  RedwoodTree::Initialize(random, txn);

  SetOverrides({BlockIds::AIR, BlockIds::BIRCH_LEAVES, BlockIds::ACACIA_LEAVES, BlockIds::GRASS, BlockIds::DIRT, BlockIds::BIRCH_LOG, BlockIds::ACACIA_LOG, BlockIds::ACACIA_SAPLING, BlockIds::VINES});
  SetHeight(static_cast<int_fast32_t>(random.NextInt(5)) + 7);
  SetLeavesHeight(height - static_cast<int_fast32_t>(random.NextInt(2)) - 3);
  SetMaxRadius(static_cast<int_fast32_t>(random.NextInt(height - leavesHeight + 1)) + 1);
}

bool TallRedwoodTree::Generate(ChunkManager &world, Random &random, int_fast32_t sourceX, int_fast32_t sourceY, int_fast32_t sourceZ) {
  if (CannotGenerateAt(sourceX, sourceY, sourceZ, world)) {
    return false;
  }

  // generate the leaves
  int_fast32_t radius = 0;
  for (int_fast32_t y = sourceY + height; y >= sourceY + leavesHeight; y--) {
    // leaves are built from top to bottom
    for (int_fast32_t x = sourceX - radius; x <= sourceX + radius; x++) {
      for (int_fast32_t z = sourceZ - radius; z <= sourceZ + radius; z++) {
        if ((abs(x - sourceX) != radius || abs(z - sourceZ) != radius || radius <= 0)  && world.GetBlockAt(x, y, z)->GetTypeId() == BlockIds::AIR) {
          transaction->AddBlockAt(x, y, z, leavesTypes);
        }
      }
    }
    if (radius >= 1 && y == sourceY + leavesHeight + 1) {
      radius--;
    } else if (radius < maxRadius) {
      radius++;
    }
  }

  // generate the trunk
  for (int_fast32_t y = 0; y < height - 1; y++) {
    ReplaceIfAirOrLeaves(sourceX, sourceY + y, sourceZ, logType, world);
  }

  // block below trunk is always dirt
  transaction->AddBlockAt(sourceX, sourceY - 1, sourceZ, MCBlock::GetBlockIdAndMeta(BlockIds::DIRT, 1));

  return true;
}



