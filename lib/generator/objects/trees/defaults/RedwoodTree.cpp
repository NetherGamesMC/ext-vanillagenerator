#include "RedwoodTree.h"

void RedwoodTree::Initialize(Random &random, BlockTransaction &txn) {
  GenericTree::Initialize(random, txn);

  SetOverrides({0, 18});
  SetHeight(static_cast<int_fast32_t>(random.NextInt(4)) + 6);
  SetLeavesHeight(static_cast<int_fast32_t>(random.NextInt(2)) + 1);
  SetMaxRadius(static_cast<int_fast32_t>(random.NextInt(2) + 2));
  SetType(MAGIC_NUMBER_SPRUCE);
}

void RedwoodTree::SetMaxRadius(int_fast32_t iMaxRadius) {
  maxRadius = iMaxRadius;
}

void RedwoodTree::SetLeavesHeight(int_fast32_t iLeavesHeight) {
  leavesHeight = iLeavesHeight;
}

bool RedwoodTree::CanPlace(int_fast32_t baseX, int_fast32_t baseY, int_fast32_t baseZ, ChunkManager &world) {
  for (int_fast32_t y = baseY; y <= baseY + 1 + height; y++) {
    // Space requirement
    int_fast32_t radius; // default radius if above first block
    if (y - baseY < leavesHeight) {
      radius = 0; // radius is 0 for trunk below leaves
    } else {
      radius = maxRadius;
    }
    // check for block collision on horizontal slices
    for (int_fast32_t x = baseX - radius; x <= baseX + radius; x++) {
      for (int_fast32_t z = baseZ - radius; z <= baseZ + radius; z++) {
        if (y >= 0 && y < 256) {
          // we can overlap some blocks around
          if (std::find(overrides.begin(), overrides.end(), world.GetBlockAt(x, y, z).GetId()) == overrides.end()) {
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

bool RedwoodTree::Generate(ChunkManager &world, Random &random, int_fast32_t sourceX, int_fast32_t sourceY, int_fast32_t sourceZ) {
  if (world.GetBlockAt(sourceX, sourceY, sourceZ) != GRASS || CannotGenerateAt(sourceX, ++sourceY, sourceZ, world)) {
    return false;
  }

  // generate the leaves
  auto radius = static_cast<int_fast32_t>(random.NextInt(2));
  int_fast32_t peakRadius = 1;
  int_fast32_t minRadius = 0;
  for (int_fast32_t y = sourceY + height; y >= sourceY + leavesHeight; y--) {
    // leaves are built from top to bottom
    for (int_fast32_t x = sourceX - radius; x <= sourceX + radius; x++) {
      for (int_fast32_t z = sourceZ - radius; z <= sourceZ + radius; z++) {
        if ((abs(x - sourceX) != radius || abs(z - sourceZ) != radius || radius <= 0) && world.GetBlockAt(x, y, z) == AIR) {
          transaction->AddBlockAt(x, y, z, leavesTypes);
        }
      }
    }
    if (radius >= peakRadius) {
      radius = minRadius;
      minRadius = 1; // after the peak radius is reached once, the min radius increases
      peakRadius++;  // the peak radius increases each time it's reached
      if (peakRadius > maxRadius) {
        peakRadius = maxRadius;
      }
    } else {
      radius++;
    }
  }

  // generate the trunk
  for (int_fast32_t y = 0; y < height - random.NextInt(3); y++) {
    const MinecraftBlock &type = world.GetBlockAt(sourceX, sourceY + y, sourceZ);
    if (std::find(overrides.begin(), overrides.end(), type.GetId()) != overrides.end()) {
      transaction->AddBlockAt(sourceX, sourceY + y, sourceZ, logType);
    }
  }

  // block below trunk is always dirt
  transaction->AddBlockAt(sourceX, sourceY - 1, sourceZ, DIRT);

  return true;
}
