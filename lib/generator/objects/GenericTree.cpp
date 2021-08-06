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
          ReplaceIfAirOrLeaves(x, y, z, leaves_types_, world);
        }
      }
    }
  }

  // generate the trunk
  for (int y = 0; y < height_; ++y) {
    ReplaceIfAirOrLeaves(sourceX, sourceY + y, sourceZ, log_type_, world);
  }

  // block below trunk is always dirt
  transaction_.AddBlockAt(sourceX, sourceY - 1, sourceZ, DIRT);

  return true;
}

bool GenericTree::CanHeightFit(int base_height) const {
  return base_height >= 1 && base_height + height_ + 1 < Y_MAX;
}

bool GenericTree::CanPlace(int_fast32_t base_x, int_fast32_t base_y, int_fast32_t base_z, ChunkManager &world) {
  for (int_fast32_t y = base_y; y <= base_y + 1 + height_; ++y) {
    // Space requirement
    int radius = 1; // default radius if above first block

    if (y == base_y) {
      radius = 0; // radius at source block y is 0 (only trunk)
    } else if (y >= base_y + 1 + height_ - 2) {
      radius = 2; // max radius starting at leaves bottom
    }

    // check for block collision on horizontal slices
    for (int_fast32_t x = base_x - radius; x <= base_x + radius; ++x) {
      for (int_fast32_t z = base_z - radius; z <= base_z + radius; ++z) {
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
    transaction_.AddBlockAt(x, y, z, newBlock);
  }
}

bool GenericTree::CanPlaceOn(MinecraftBlock soil) {
  uint_fast32_t type = soil.GetId();

  return type == GRASS.GetId() || type == DIRT.GetId() || type == FARMLAND.GetId();
}

bool GenericTree::CannotGenerateAt(int_fast32_t base_x, int_fast32_t base_y, int_fast32_t base_z, ChunkManager &world) {
  return !CanHeightFit(base_y) || !CanPlaceOn(world.GetBlockAt(base_x, base_y - 1, base_z)) || !CanPlace(base_x, base_y, base_z, world);
}

GenericTree::GenericTree(Random &random, BlockTransaction &txn) : transaction_(txn) {
  height_ = random.NextInt(3) + 4;
}