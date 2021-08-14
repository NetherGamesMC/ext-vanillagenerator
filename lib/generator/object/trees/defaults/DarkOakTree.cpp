#include "DarkOakTree.h"

void DarkOakTree::Initialize(Random &random, BlockTransaction &txn) {
  GenericTree::Initialize(random, txn);

  SetOverrides({0, 18, 161, 2, 3, 17, 162, 6, 106});
  SetHeight(static_cast<int_fast32_t>(random.NextInt(2) + random.NextInt(3)) + 6);
  SetType(MAGIC_NUMBER_DARK_OAK);
}

bool DarkOakTree::CanPlaceOn(MinecraftBlock soil) {
  return soil == GRASS || soil == DIRT;
}

void DarkOakTree::SetLeaves(int_fast32_t x, int_fast32_t y, int_fast32_t z, ChunkManager &world) {
  if (world.GetBlockAt(x, y, z) == AIR) {
    transaction->AddBlockAt(x, y, z, leavesTypes);
  }
}

bool DarkOakTree::Generate(ChunkManager &world, Random &random, int_fast32_t sourceX, int_fast32_t sourceY, int_fast32_t sourceZ) {
  if (CannotGenerateAt(sourceX, sourceY, sourceZ, world)) {
    return false;
  }

  auto d = (float) (random.NextFloat() * M_PI * 2.0F); // random direction
  int_fast32_t dx = (int) (cos(d) + 1.5F) - 1;
  int_fast32_t dz = (int) (sin(d) + 1.5F) - 1;
  if (abs(dx) > 0 && abs(dz) > 0) { // reduce possible directions to NESW
    if (random.NextBoolean()) {
      dx = 0;
    } else {
      dz = 0;
    }
  }

  int_fast32_t twistHeight = height - static_cast<int_fast32_t>(random.NextInt(4));
  int_fast32_t twistCount = static_cast<int_fast32_t>(random.NextInt(3));
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
    if (material != AIR && material.GetId() != 18) {
      continue;
    }
    trunkTopY = sourceY + y;
    // SELF, SOUTH, EAST, SOUTH EAST
    transaction->AddBlockAt(centerX, sourceY + y, centerZ, logType);
    transaction->AddBlockAt(centerX, sourceY + y, centerZ + 1, logType);
    transaction->AddBlockAt(centerX + 1, sourceY + y, centerZ, logType);
    transaction->AddBlockAt(centerX + 1, sourceY + y, centerZ + 1, logType);
  }

  // generates leaves
  for (int_fast32_t x = -2; x <= 0; x++) {
    for (int_fast32_t z = -2; z <= 0; z++) {
      if ((x != -1 || z != -2) && (x > -2 || z > -1)) {
        SetLeaves(centerX + x, trunkTopY + 1, centerZ + z, world);
        SetLeaves(1 + centerX - x, trunkTopY + 1, centerZ + z, world);
        SetLeaves(centerX + x, trunkTopY + 1, 1 + centerZ - z, world);
        SetLeaves(1 + centerX - x, trunkTopY + 1, 1 + centerZ - z, world);
      }
      SetLeaves(centerX + x, trunkTopY - 1, centerZ + z, world);
      SetLeaves(1 + centerX - x, trunkTopY - 1, centerZ + z, world);
      SetLeaves(centerX + x, trunkTopY - 1, 1 + centerZ - z, world);
      SetLeaves(1 + centerX - x, trunkTopY - 1, 1 + centerZ - z, world);
    }
  }

  // finish leaves below the canopy
  for (int_fast32_t x = -3; x <= 4; x++) {
    for (int_fast32_t z = -3; z <= 4; z++) {
      if (abs(x) < 3 || abs(z) < 3) {
        SetLeaves(centerX + x, trunkTopY, centerZ + z, world);
      }
    }
  }

  // generates some trunk excrescences
  for (int_fast32_t x = -1; x <= 2; x++) {
    for (int_fast32_t z = -1; z <= 2; z++) {
      if ((x != -1 && z != -1 && x != 2 && z != 2) || random.NextInt(3) != 0) {
        continue;
      }
      for (int_fast32_t y = 0; y < random.NextInt(3) + 2; y++) {
        const MinecraftBlock &material = world.GetBlockAt(sourceX + x, trunkTopY - y - 1, sourceZ + z);
        if (material == AIR || material.GetId() == 18) {
          transaction->AddBlockAt(sourceX + x, trunkTopY - y - 1, sourceZ + z, logType);
        }
      }

      // leaves below the canopy
      for (int_fast32_t i = -1; i <= 1; i++) {
        for (int_fast32_t j = -1; j <= 1; j++) {
          SetLeaves(centerX + x + i, trunkTopY, centerZ + z + j, world);
        }
      }
      for (int_fast32_t i = -2; i <= 2; i++) {
        for (int_fast32_t j = -2; j <= 2; j++) {
          if (abs(i) < 2 || abs(j) < 2) {
            SetLeaves(centerX + x + i, trunkTopY - 1, centerZ + z + j, world);
          }
        }
      }
    }
  }

  // 50% chance to have a 4 leaves cap on the center of the canopy
  if (random.NextInt(2) == 0) {
    SetLeaves(centerX, trunkTopY + 2, centerZ, world);
    SetLeaves(centerX + 1, trunkTopY + 2, centerZ, world);
    SetLeaves(centerX + 1, trunkTopY + 2, centerZ + 1, world);
    SetLeaves(centerX, trunkTopY + 2, centerZ + 1, world);
  }

  // block below trunk is always dirt (SELF, SOUTH, EAST, SOUTH EAST)
  transaction->AddBlockAt(sourceX, sourceY - 1, sourceZ, DIRT);
  transaction->AddBlockAt(sourceX, sourceY - 1, sourceZ + 1, DIRT);
  transaction->AddBlockAt(sourceX + 1, sourceY - 1, sourceZ, DIRT);
  transaction->AddBlockAt(sourceX + 1, sourceY - 1, sourceZ + 1, DIRT);

  return true;
}


