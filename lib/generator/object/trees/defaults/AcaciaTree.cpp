#include "AcaciaTree.h"

void AcaciaTree::Initialize(Random &random, BlockTransaction &txn) {
  GenericTree::Initialize(random, txn);

  SetHeight(static_cast<int_fast32_t>(random.NextInt(3) + random.NextInt(3)) + 5);
  SetType(MAGIC_NUMBER_ACACIA);
}

bool AcaciaTree::CanPlaceOn(MinecraftBlock soil) {
  return soil == GRASS || soil == DIRT;
}

void AcaciaTree::SetLeaves(int_fast32_t x, int_fast32_t y, int_fast32_t z, ChunkManager &world) {
  if (world.GetBlockAt(x, y, z) == AIR) {
    transaction->AddBlockAt(x, y, z, leavesTypes);
  }
}

bool AcaciaTree::Generate(ChunkManager &world, Random &random, int_fast32_t sourceX, int_fast32_t sourceY, int_fast32_t sourceZ) {
  if (CannotGenerateAt(sourceX, sourceY, sourceZ, world)) {
    return false;
  }

  auto d = (float) (random.NextFloat() * M_PI * 2.0F); // random direction
  int_fast32_t dx = (int) (cos(d) + 1.5F) - 1;
  int_fast32_t dz = (int) (sin(d) + 1.5F) - 1;
  if (abs(dx) > 0 && abs(dz) > 0) { // reduce possible directions to NESW
    if (random.NextInt()) {
      dx = 0;
    } else {
      dz = 0;
    }
  }

  int_fast32_t twistHeight = height - 1 - static_cast<int_fast32_t>(random.NextInt(4));
  int_fast32_t twistCount = static_cast<int_fast32_t>(random.NextInt(3)) + 1;
  int_fast32_t centerX = sourceX;
  int_fast32_t centerZ = sourceZ;
  int_fast32_t trunkTopY = 0;

  // generates the trunk
  for (int_fast32_t y = 0; y < height; y++) {

    // trunk twists
    if (twistCount > 0 && y >= twistHeight) {
      centerX += dx;
      centerZ += dz;
      twistCount--;
    }

    const MinecraftBlock &material = world.GetBlockAt(centerX, sourceY + y, centerZ);
    if (material == AIR || material.GetId() == 18) {
      trunkTopY = sourceY + y;
      transaction->AddBlockAt(centerX, sourceY + y, centerZ, logType);
    }
  }

  // generates leaves
  for (int_fast32_t x = -3; x <= 3; x++) {
    for (int_fast32_t z = -3; z <= 3; z++) {
      if (abs(x) < 3 || abs(z) < 3) {
        SetLeaves(centerX + x, trunkTopY, centerZ + z, world);
      }
      if (abs(x) < 2 && abs(z) < 2) {
        SetLeaves(centerX + x, trunkTopY + 1, centerZ + z, world);
      }
      if (abs(x) == 2 && abs(z) == 0 || abs(x) == 0 && abs(z) == 2) {
        SetLeaves(centerX + x, trunkTopY + 1, centerZ + z, world);
      }
    }
  }

  // try to choose a different direction for second branching and canopy
  d = (float) (random.NextFloat() * M_PI * 2.0F);
  int_fast32_t dxB = (int) (cos(d) + 1.5F) - 1;
  int_fast32_t dzB = (int) (sin(d) + 1.5F) - 1;
  if (abs(dxB) > 0 && abs(dzB) > 0) {
    if (random.NextBoolean()) {
      dxB = 0;
    } else {
      dzB = 0;
    }
  }

  if (dx != dxB || dz != dzB) {
    centerX = sourceX;
    centerZ = sourceZ;
    int_fast32_t branchHeight = twistHeight - 1 - static_cast<int_fast32_t>(random.NextInt(2));
    twistCount = static_cast<int_fast32_t>(random.NextInt(3)) + 1;
    trunkTopY = 0;

    // generates the trunk
    for (int_fast32_t y = branchHeight + 1; y < height; y++) {
      if (twistCount > 0) {
        centerX += dxB;
        centerZ += dzB;
        const MinecraftBlock &material = world.GetBlockAt(centerX, sourceY + y, centerZ);
        if (material == AIR || material.GetId() == 18) {
          trunkTopY = sourceY + y;
          transaction->AddBlockAt(centerX, sourceY + y, centerZ, logType);
        }
        twistCount--;
      }
    }

    // generates the leaves
    if (trunkTopY > 0) {
      for (int_fast32_t x = -2; x <= 2; x++) {
        for (int_fast32_t z = -2; z <= 2; z++) {
          if (abs(x) < 2 || abs(z) < 2) {
            SetLeaves(centerX + x, trunkTopY, centerZ + z, world);
          }
        }
      }
      for (int_fast32_t x = -1; x <= 1; x++) {
        for (int_fast32_t z = -1; z <= 1; z++) {
          SetLeaves(centerX + x, trunkTopY + 1, centerZ + z, world);
        }
      }
    }
  }

  return true;
}
