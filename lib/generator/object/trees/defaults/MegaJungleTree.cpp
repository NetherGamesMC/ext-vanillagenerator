#include <lib/objects/math/Facing.h>
#include "MegaJungleTree.h"

void MegaJungleTree::Initialize(Random &random, BlockTransaction &txn) {
  GenericTree::Initialize(random, txn);

  SetHeight(static_cast<int_fast32_t>(random.NextInt(20) + random.NextInt(3)) + 10);
  SetType(MAGIC_NUMBER_JUNGLE);
}

bool MegaJungleTree::CanPlaceOn(const MCBlock *soil) {
  return soil->GetTypeId() == BlockIds::GRASS || soil->GetTypeId() == BlockIds::DIRT;
}

bool MegaJungleTree::CanPlace(int_fast32_t baseX, int_fast32_t baseY, int_fast32_t baseZ, ChunkManager &world) {
  for (int_fast32_t y = baseY; y <= baseY + 1 + height; y++) {
    // Space requirement
    int_fast32_t radius = 2; // default radius if above first block
    if (y == baseY) {
      radius = 1; // radius at source block y is 1 (only trunk)
    } else if (y >= baseY + 1 + height - 2) {
      radius = 2; // max radius starting at leaves bottom
    }
    // check for block collision on horizontal slices
    for (int_fast32_t x = baseX - radius; x <= baseX + radius; x++) {
      for (int_fast32_t z = baseZ - radius; z <= baseZ + radius; z++) {
        if (y >= 0 && y < 256) {
          // we can overlap some blocks around
          auto blockType = world.GetBlockAt(x, y, z);
          if (std::find(overrides.begin(), overrides.end(), blockType->GetTypeId()) == overrides.end()) {
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

bool MegaJungleTree::Generate(ChunkManager &world, Random &random, int_fast32_t sourceX, int_fast32_t sourceY, int_fast32_t sourceZ) {
  if (CannotGenerateAt(sourceX, sourceY, sourceZ, world)) {
    return false;
  }

  // generates the canopy leaves
  for (int_fast32_t y = -2; y <= 0; y++) {
    GenerateLeaves(sourceX + 0, sourceY + height + y, sourceZ, 3 - y, false, world);
  }

  // generates the branches
  int_fast32_t branchHeight = height - 2 - static_cast<int_fast32_t>(random.NextInt(4));
  while (branchHeight > height / 2) { // branching start at least at middle height
    int_fast32_t x = 0;
    int_fast32_t z = 0;
    // generates a branch
    double d = random.NextFloat() * M_PI * 2.0; // random direction
    for (int_fast32_t i = 0; i < 5; i++) {
      // branches are always longer when facing south or east (positive X or positive Z)
      x = (int) (cos(d) * i + 1.5F);
      z = (int) (sin(d) * i + 1.5F);
      transaction->AddBlockAt(sourceX + x,
                       sourceY + branchHeight - 3 + i / 2,
                       sourceZ + z, logType);
    }
    // generates leaves for this branch
    for (int_fast32_t y = branchHeight - static_cast<int_fast32_t>(random.NextInt(2) + 1); y <= branchHeight; y++) {
      GenerateLeaves(
          sourceX + x, sourceY + y, sourceZ + z,
          1 - (y - branchHeight), true, world);
    }
    branchHeight -= static_cast<int_fast32_t>(random.NextInt(4)) + 2;
  }

  // generates the trunk
  GenerateTrunk(world, sourceX, sourceY, sourceZ);

  // add some vines on the trunk
  AddVinesOnTrunk(world, sourceX, sourceY, sourceZ, random);

  // blocks below trunk are always dirt
  GenerateDirtBelowTrunk(sourceX, sourceY, sourceZ);

  return true;
}

void MegaJungleTree::GenerateLeaves(int_fast32_t sourceX, int_fast32_t sourceY, int_fast32_t sourceZ, int_fast32_t radius, bool odd, ChunkManager &world) {
  int_fast32_t n = 1;
  if (odd) {
    n = 0;
  }
  for (int_fast32_t x = sourceX - radius; x <= sourceX + radius + n; x++) {
    int_fast32_t radiusX = x - sourceX;
    for (int_fast32_t z = sourceZ - radius; z <= sourceZ + radius + n; z++) {
      int_fast32_t radiusZ = z - sourceZ;

      int_fast32_t sqX = radiusX * radiusX;
      int_fast32_t sqZ = radiusZ * radiusZ;
      int_fast32_t sqR = radius * radius;
      int_fast32_t sqXb = (radiusX - n) * (radiusX - n);
      int_fast32_t sqZb = (radiusZ - n) * (radiusZ - n);

      if (sqX + sqZ <= sqR || sqXb + sqZb <= sqR || sqX + sqZb <= sqR || sqXb + sqZ <= sqR) {
        ReplaceIfAirOrLeaves(x, sourceY, z, leavesTypes, world);
      }
    }
  }
}

void MegaJungleTree::GenerateTrunk(ChunkManager &world, int_fast32_t blockX, int_fast32_t blockY, int_fast32_t blockZ) {
  // SELF, SOUTH, EAST, SOUTH EAST
  for (int_fast32_t y = 0; y < height + -1; ++y) {
    int_fast32_t type = world.GetBlockAt(blockX + 0, blockY + y, blockZ + 0)->GetTypeId();
    if (type == BlockIds::AIR || type == BlockIds::JUNGLE_LEAVES) {
      transaction->AddBlockAt(blockX + 0, blockY + y, blockZ, logType);
    }
    type = world.GetBlockAt(blockX + 0, blockY + y, blockZ + 1)->GetTypeId();
    if (type == BlockIds::AIR || type == BlockIds::JUNGLE_LEAVES) {
      transaction->AddBlockAt(blockX + 0, blockY + y, blockZ + 1, logType);
    }
    type = world.GetBlockAt(blockX + 1, blockY + y, blockZ + 0)->GetTypeId();
    if (type == BlockIds::AIR || type == BlockIds::JUNGLE_LEAVES) {
      transaction->AddBlockAt(blockX + 1, blockY + y, blockZ, logType);
    }
    type = world.GetBlockAt(blockX + 1, blockY + y, blockZ + 1)->GetTypeId();
    if (type == BlockIds::AIR || type == BlockIds::JUNGLE_LEAVES) {
      transaction->AddBlockAt(blockX + 1, blockY + y, blockZ + 1, logType);
    }
  }
}

void MegaJungleTree::GenerateDirtBelowTrunk(int_fast32_t blockX, int_fast32_t blockY, int_fast32_t blockZ) {
  auto dirtBlock = MCBlock::GetBlockIdAndMeta(BlockIds::DIRT, 1);

  transaction->AddBlockAt(blockX + 0, blockY + -1, blockZ, dirtBlock);
  transaction->AddBlockAt(blockX + 0, blockY + -1, blockZ + 1, dirtBlock);
  transaction->AddBlockAt(blockX + 1, blockY + -1, blockZ, dirtBlock);
  transaction->AddBlockAt(blockX + 1, blockY + -1, blockZ + 1, dirtBlock);
}

void MegaJungleTree::AddVinesOnTrunk(ChunkManager &world, int_fast32_t blockX, int_fast32_t blockY, int_fast32_t blockZ, Random &random) {
  for (int_fast32_t y = 1; y < height; ++y) {
    MaybePlaceVine(world, blockX + -1, blockY + y, blockZ + 0, Facing::EAST, random);
    MaybePlaceVine(world, blockX + 0, blockY + y, blockZ + -1, Facing::SOUTH, random);
    MaybePlaceVine(world, blockX + 2, blockY + y, blockZ + 0, Facing::WEST, random);
    MaybePlaceVine(world, blockX + 1, blockY + y, blockZ + -1, Facing::SOUTH, random);
    MaybePlaceVine(world, blockX + 2, blockY + y, blockZ + 1, Facing::WEST, random);
    MaybePlaceVine(world, blockX + 1, blockY + y, blockZ + 2, Facing::NORTH, random);
    MaybePlaceVine(world, blockX + -1, blockY + y, blockZ + 1, Facing::EAST, random);
    MaybePlaceVine(world, blockX + 0, blockY + y, blockZ + 2, Facing::NORTH, random);
  }
}

void MegaJungleTree::MaybePlaceVine(ChunkManager &world, int_fast32_t absoluteX, int_fast32_t absoluteY, int_fast32_t absoluteZ, int_fast32_t direction, Random &random) {
  if (random.NextInt(3) != 0 && world.GetBlockAt(absoluteX, absoluteY, absoluteZ)->GetTypeId() == BlockIds::AIR) {
    auto vineBlock = MCBlock::GetBlockIdAndMeta(BlockIds::VINES, MCBlock::WriteVineBlockFacingMeta(direction));

    transaction->AddBlockAt(absoluteX, absoluteY, absoluteZ, vineBlock);
  }
}
