#include "JungleBush.h"

void JungleBush::Initialize(Random &random, BlockTransaction &txn) {
  GenericTree::Initialize(random, txn);

  SetType(MAGIC_NUMBER_JUNGLE);
}

bool JungleBush::CanPlaceOn(MinecraftBlock soil) {
  return soil == GRASS || soil == DIRT;
}

bool JungleBush::Generate(ChunkManager &world, Random &random, int_fast32_t sourceX, int_fast32_t sourceY, int_fast32_t sourceZ) {
  int_fast32_t blockId;
  while (((blockId = world.GetBlockAt(sourceX, sourceY, sourceZ).GetId()) == AIR.GetId() || blockId == 18) && sourceY > 0) {
    --sourceY;
  }

  // check only below block
  if (!CanPlaceOn(world.GetBlockAt(sourceX, sourceY - 1, sourceZ))) {
    return false;
  }

  // generates the trunk
  int_fast32_t adjust_y = sourceY;
  transaction->AddBlockAt(sourceX, adjust_y + 1, sourceZ, logType);

  // generates the leaves
  for (int_fast32_t y = adjust_y + 1; y <= adjust_y + 3; ++y) {
    int_fast32_t radius = 3 - (y - adjust_y);

    for (int_fast32_t x = sourceX - radius; x <= sourceX + radius; ++x) {
      for (int_fast32_t z = sourceZ - radius; z <= sourceZ + radius; ++z) {
        if (!IS_SOLID(transaction->FetchBlockAt(x, y, z).GetId()) &&
            (abs(x - sourceX) != radius || abs(z - sourceZ) != radius || random.NextBoolean())) {
          transaction->AddBlockAt(x, y, z, leavesTypes);
        }
      }
    }
  }

  return true;
}




