#include "SwampTree.h"

void SwampTree::Initialize(Random &random, BlockTransaction &txn) {
  JungleTree::Initialize(random, txn);

  SetOverrides({BlockIds::AIR, BlockIds::JUNGLE_LEAVES});
  SetHeight(random.NextInt(4) + 5);
  SetType(MAGIC_NUMBER_OAK);
}

bool SwampTree::CanPlaceOn(const MCBlock *soil) {
  return soil->GetTypeId() == BlockIds::GRASS || soil->GetTypeId() == BlockIds::DIRT;
}

bool SwampTree::CanPlace(int_fast32_t baseX, int_fast32_t baseY, int_fast32_t baseZ, ChunkManager &world) {
  for (int_fast32_t y = baseY; y <= baseY + 1 + height; y++) {
    if (y < 0 || y >= 256) { // height out of range
      return false;
    }
    // Space requirement
    int_fast32_t radius = 1; // default radius if above first block
    if (y == baseY) {
      radius = 0; // radius at source block y is 0 (only trunk)
    } else if (y >= baseY + 1 + height - 2) {
      radius = 3; // max radius starting at leaves bottom
    }
    // check for block collision on horizontal slices
    for (int_fast32_t x = baseX - radius; x <= baseX + radius; x++) {
      for (int_fast32_t z = baseZ - radius; z <= baseZ + radius; z++) {
        // we can overlap some blocks around
        auto blockType = world.GetBlockAt(x, y, z);
        if (std::find(overrides.begin(), overrides.end(), blockType->GetTypeId()) != overrides.end()) {
          continue;
        }
        // the trunk can be immersed by 1 block of water
        if (blockType->GetTypeId() == BlockIds::WATER) {
          if (y > baseY) {
            return false;
          }
        } else {
          return false;
        }
      }
    }
  }
  return true;
}

bool SwampTree::Generate(ChunkManager &world, Random &random, int_fast32_t sourceX, int_fast32_t sourceY, int_fast32_t sourceZ) {
  while (world.GetBlockAt(sourceX, sourceY, sourceZ)->GetTypeId() == BlockIds::WATER) {
    sourceY--;
  }

  ++sourceY;
  if (CannotGenerateAt(sourceX, sourceY, sourceZ, world)) {
    return false;
  }

  // generate the leaves
  for (int_fast32_t y = sourceY + height - 3; y <= sourceY + height; y++) {
    int_fast32_t n = y - (sourceY + height);
    int_fast32_t radius = 2 - n / 2;
    for (int_fast32_t x = sourceX - radius; x <= sourceX + radius; x++) {
      for (int_fast32_t z = sourceZ - radius; z <= sourceZ + radius; z++) {
        if (abs(x - sourceX) != radius || abs(z - sourceZ) != radius || random.NextBoolean() && n != 0) {
          ReplaceIfAirOrLeaves(x, y, z, leavesTypes, world);
        }
      }
    }
  }

  // generate the trunk
  for (int_fast32_t y = 0; y < height; y++) {
    auto material = world.GetBlockAt(sourceX, sourceY + y, sourceZ)->GetTypeId();
    if (material == BlockIds::AIR || material == BlockIds::JUNGLE_LEAVES || material == BlockIds::WATER) {
      transaction->AddBlockAt(sourceX, sourceY + y, sourceZ, logType);
    }
  }

  // add some vines on the leaves
  AddVinesOnLeaves(sourceX, sourceY, sourceZ, world, random);

  // block below trunk is always dirt
  transaction->AddBlockAt(sourceX, sourceY - 1, sourceZ, MCBlock::GetBlockIdAndMeta(BlockIds::DIRT, 1));

  return true;
}

