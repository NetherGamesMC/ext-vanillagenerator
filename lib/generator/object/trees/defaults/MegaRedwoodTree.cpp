#include "MegaRedwoodTree.h"

void MegaRedwoodTree::Initialize(Random &random, BlockTransaction &txn) {
  MegaJungleTree::Initialize(random, txn);

  SetHeight(static_cast<int_fast32_t>(random.NextInt(15) + random.NextInt(3)) + 13);
  SetType(MAGIC_NUMBER_SPRUCE);
  SetLeavesHeight(static_cast<int_fast32_t>(random.NextInt(5)) + (random.NextBoolean() ? 3 : 13));
}

void MegaRedwoodTree::SetLeavesHeight(int_fast32_t iLeavesHeight) {
  this->leavesHeight = iLeavesHeight;
}

bool MegaRedwoodTree::Generate(ChunkManager &world, Random &random, int_fast32_t sourceX, int_fast32_t sourceY, int_fast32_t sourceZ) {
  if (CannotGenerateAt(sourceX, sourceY, sourceZ, world)) {
    return false;
  }

  // generates the leaves
  int_fast32_t previousRadius = 0;
  for (int_fast32_t y = sourceY + height - leavesHeight; y <= sourceY + height; y++) {
    int_fast32_t n = sourceY + height - y;
    int_fast32_t radius = (int) std::floor((double) n / leavesHeight * 3.5);
    if (radius == previousRadius && n > 0 && y % 2 == 0) {
      radius++;
    }
    GenerateLeaves(sourceX, y, sourceZ, radius, false, world);
    previousRadius = radius;
  }

  // generates the trunk
  GenerateTrunk(world, sourceX, sourceY, sourceZ);

  // blocks below trunk are always dirt
  GenerateDirtBelowTrunk(sourceX, sourceY, sourceZ);

  return true;
}

void MegaRedwoodTree::GenerateDirtBelowTrunk(int_fast32_t blockX, int_fast32_t blockY, int_fast32_t blockZ) {
  // NOOP: MegaRedwoodTree does not replace blocks below (surely to preserves podzol)
}

