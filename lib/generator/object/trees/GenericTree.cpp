#include "GenericTree.h"

bool GenericTree::Generate(ChunkManager &world, Random &random, int_fast32_t sourceX, int_fast32_t sourceY, int_fast32_t sourceZ) {
  if (CannotGenerateAt(sourceX, sourceY, sourceZ, world)) {
    return false;
  }

  // generate the leaves
  for (auto y = sourceY + height - 3; y <= sourceY + height; ++y) {
    int_fast32_t n = y - (sourceY + height);
    int_fast32_t radius = (int) (1 - n / 2);

    for (int_fast32_t x = sourceX - radius; x <= sourceX + radius; ++x) {
      for (int_fast32_t z = sourceZ - radius; z <= sourceZ + radius; ++z) {
        if (abs(x - sourceX) != radius || abs(z - sourceZ) != radius || (random.NextBoolean() && n != 0)) {
          ReplaceIfAirOrLeaves(x, y, z, leavesTypes, world);
        }
      }
    }
  }

  // generate the trunk
  for (int_fast32_t y = 0; y < height; ++y) {
    ReplaceIfAirOrLeaves(sourceX, sourceY + y, sourceZ, logType, world);
  }

  // block below trunk is always dirt
  transaction->AddBlockAt(sourceX, sourceY - 1, sourceZ, MCBlock::GetBlockIdAndMeta(BlockIds::DIRT, 1));

  return true;
}

bool GenericTree::CanHeightFit(int_fast32_t baseHeight) const {
  return baseHeight >= 1 && baseHeight + height + 1 < Chunk::Y_MAX;
}

bool GenericTree::CanPlace(int_fast32_t baseX, int_fast32_t baseY, int_fast32_t baseZ, ChunkManager &world) {
  for (int_fast32_t y = baseY; y <= baseY + 1 + height; ++y) {
    // Space requirement
    int_fast32_t radius = 1; // default radius if above first block

    if (y == baseY) {
      radius = 0; // radius at source block y is 0 (only trunk)
    } else if (y >= baseY + 1 + height - 2) {
      radius = 2; // max radius starting at leaves bottom
    }

    // check for block collision on horizontal slices
    for (int_fast32_t x = baseX - radius; x <= baseX + radius; ++x) {
      for (int_fast32_t z = baseZ - radius; z <= baseZ + radius; ++z) {
        if (y >= 0 && y < world.GetMaxY()) {
          // we can overlap some blocks around
          if (std::find(overrides.begin(), overrides.end(), world.GetBlockAt(x, y, z)->GetTypeId()) == overrides.end()) {
            return false;
          }
        } else { // height out of range
          return false;
        }
      }
    }
  }

  return true;
}

void GenericTree::ReplaceIfAirOrLeaves(int_fast32_t x, int_fast32_t y, int_fast32_t z, const MCBlock *newBlock, ChunkManager &world) {
  auto oldBlock = world.GetBlockAt(x, y, z)->GetTypeId();
  if (oldBlock == BlockIds::AIR || oldBlock == BlockIds::ACACIA_LEAVES || oldBlock == BlockIds::BIRCH_LEAVES || oldBlock == BlockIds::DARK_OAK_LEAVES
      || oldBlock == BlockIds::JUNGLE_LEAVES || oldBlock == BlockIds::OAK_LEAVES || oldBlock == BlockIds::SPRUCE_LEAVES) {
    transaction->AddBlockAt(x, y, z, newBlock);
  }
}

bool GenericTree::CanPlaceOn(const MCBlock *soil) {
  uint_fast32_t type = soil->GetTypeId();
  return type == BlockIds::GRASS || type == BlockIds::DIRT || type == BlockIds::FARMLAND;
}

bool GenericTree::CannotGenerateAt(int_fast32_t baseX, int_fast32_t baseY, int_fast32_t baseZ, ChunkManager &world) {
  return !CanHeightFit(baseY) || !CanPlaceOn(world.GetBlockAt(baseX, baseY - 1, baseZ)) || !CanPlace(baseX, baseY, baseZ, world);
}

void GenericTree::Initialize(Random &random, BlockTransaction &txn) {
  height = static_cast<int>(random.NextInt(3)) + 4;
  transaction = &txn;
}

void GenericTree::SetHeight(int_fast32_t blockHeight) { height = blockHeight; }

void GenericTree::SetOverrides(std::vector<int> overridable) { overrides = std::move(overridable); }

void GenericTree::SetType(int_fast32_t magicNumber) {
  int logBlockId = BlockIds::AIR;
  int leavesBlockId = BlockIds::AIR;

  switch (magicNumber) {
    case 0:
      logBlockId = BlockIds::OAK_LOG;
      leavesBlockId = BlockIds::OAK_LEAVES;
      break;
    case 1:
      logBlockId = BlockIds::SPRUCE_LOG;
      leavesBlockId = BlockIds::SPRUCE_LEAVES;
      break;
    case 2:
      logBlockId = BlockIds::BIRCH_LOG;
      leavesBlockId = BlockIds::BIRCH_LEAVES;
      break;
    case 3:
      logBlockId = BlockIds::JUNGLE_LOG;
      leavesBlockId = BlockIds::JUNGLE_LEAVES;
      break;
    case 4:
      logBlockId = BlockIds::ACACIA_LOG;
      leavesBlockId = BlockIds::ACACIA_LEAVES;
      break;
    case 5:
      logBlockId = BlockIds::DARK_OAK_LOG;
      leavesBlockId = BlockIds::DARK_OAK_LEAVES;
      break;
  }

  logType = MCBlock::GetBlockIdAndMeta(logBlockId, 4);
  leavesTypes = MCBlock::GetBlockIdAndMeta(leavesBlockId, 0);
}

static GenericTree OakTree(Random &random, BlockTransaction &txn) { return {}; }