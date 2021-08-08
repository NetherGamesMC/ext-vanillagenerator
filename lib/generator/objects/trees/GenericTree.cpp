#include <lib/objects/constants/Logic.h>
#include "GenericTree.h"

bool GenericTree::Generate(ChunkManager &world, Random &random, int_fast32_t sourceX, int_fast32_t sourceY, int_fast32_t sourceZ) {
  if (CannotGenerateAt(sourceX, sourceY, sourceZ, world)) {
    return false;
  }

  // generate the leaves
  for (auto y = sourceY + height_ - 3; y <= sourceY + height_; ++y) {
    int n = y - (sourceY + height_);
    int radius = (int) (1 - n / 2);

    for (int_fast32_t x = sourceX - radius; x <= sourceX + radius; ++x) {
      for (int_fast32_t z = sourceZ - radius; z <= sourceZ + radius; ++z) {
        if (abs(x - sourceX) != radius || abs(z - sourceZ) != radius || (random.NextBoolean() && n != 0)) {
          ReplaceIfAirOrLeaves(x, y, z, leavesTypes_, world);
        }
      }
    }
  }

  // generate the trunk
  for (int y = 0; y < height_; ++y) {
    ReplaceIfAirOrLeaves(sourceX, sourceY + y, sourceZ, logType_, world);
  }

  // block below trunk is always dirt
  (*transaction_).AddBlockAt(sourceX, sourceY - 1, sourceZ, DIRT);

  return true;
}

bool GenericTree::CanHeightFit(int baseHeight) const {
  return baseHeight >= 1 && baseHeight + height_ + 1 < Y_MAX;
}

bool GenericTree::CanPlace(int_fast32_t baseX, int_fast32_t baseY, int_fast32_t baseZ, ChunkManager &world) {
  for (int_fast32_t y = baseY; y <= baseY + 1 + height_; ++y) {
    // Space requirement
    int radius = 1; // default radius if above first block

    if (y == baseY) {
      radius = 0; // radius at source block y is 0 (only trunk)
    } else if (y >= baseY + 1 + height_ - 2) {
      radius = 2; // max radius starting at leaves bottom
    }

    // check for block collision on horizontal slices
    for (int_fast32_t x = baseX - radius; x <= baseX + radius; ++x) {
      for (int_fast32_t z = baseZ - radius; z <= baseZ + radius; ++z) {
        if (y >= 0 && y < world.GetMaxY()) {
          // we can overlap some blocks around
          if (std::find(overrides_.begin(), overrides_.end(), world.GetBlockAt(x, y, z).GetId()) == overrides_.end()) {
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

void GenericTree::ReplaceIfAirOrLeaves(int_fast32_t x, int_fast32_t y, int_fast32_t z, MinecraftBlock newBlock, ChunkManager &world) {
  auto oldBlock = world.GetBlockAt(x, y, z).GetId();
  if (oldBlock == 0 || oldBlock == 18) {
    (*transaction_).AddBlockAt(x, y, z, newBlock);
  }
}

bool GenericTree::CanPlaceOn(MinecraftBlock soil) {
  uint_fast32_t type = soil.GetId();

  return type == GRASS.GetId() || type == DIRT.GetId() || type == FARMLAND.GetId();
}

bool GenericTree::CannotGenerateAt(int_fast32_t baseX, int_fast32_t baseY, int_fast32_t baseZ, ChunkManager &world) {
  return !CanHeightFit(baseY) || !CanPlaceOn(world.GetBlockAt(baseX, baseY - 1, baseZ)) || !CanPlace(baseX, baseY, baseZ, world);
}

void GenericTree::Initialize(Random &random, BlockTransaction &txn) {
  height_ = static_cast<int>(random.NextInt(3)) + 4;
  transaction_ = &txn;
}

void GenericTree::SetHeight(int blockHeight) { height_ = blockHeight; }

void GenericTree::SetOverrides(std::vector<int> overridable) { overrides_ = std::move(overridable); }

void GenericTree::SetType(int magicNumber) {
  logType_ = MinecraftBlock(magicNumber >= 4 ? 162 : 17, magicNumber & 0x3);
  leavesTypes_ = MinecraftBlock(magicNumber >= 4 ? 161 : 18, magicNumber & 0x3);
}

static GenericTree OakTree(Random &random, BlockTransaction &txn) { return {}; }