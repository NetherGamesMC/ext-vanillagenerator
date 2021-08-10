#include "MegaPineTree.h"

void MegaPineTree::Initialize(Random &random, BlockTransaction &txn) {
  MegaRedwoodTree::Initialize(random, txn);

  SetLeavesHeight(static_cast<int_fast32_t>(random.NextInt(5)) + 3);
}

bool MegaPineTree::Generate(ChunkManager &world, Random &random, int_fast32_t sourceX, int_fast32_t sourceY, int_fast32_t sourceZ) {
  bool generated = MegaRedwoodTree::Generate(world, random, sourceX, sourceY, sourceZ);
  if (generated) {
    GeneratePodzol(sourceX, sourceY, sourceZ, world, random);
  }

  return generated;
}

void MegaPineTree::GenerateDirtBelowTrunk(int_fast32_t blockX, int_fast32_t blockY, int_fast32_t blockZ) {
  transaction->AddBlockAt(blockX + 0, blockY + -1, blockZ, PODZOL);
  transaction->AddBlockAt(blockX + 0, blockY + -1, blockZ + 1, PODZOL);
  transaction->AddBlockAt(blockX + 1, blockY + -1, blockZ, PODZOL);
  transaction->AddBlockAt(blockX + 1, blockY + -1, blockZ + 1, PODZOL);
}

void MegaPineTree::GeneratePodzol(int_fast32_t sourceX, int_fast32_t sourceY, int_fast32_t sourceZ, ChunkManager &world, Random &random) {
  GeneratePodzolPatch(sourceX - 1, sourceY, sourceZ - 1, world);
  GeneratePodzolPatch(sourceX + 2, sourceY, sourceZ - 1, world);
  GeneratePodzolPatch(sourceX - 1, sourceY, sourceZ + 2, world);
  GeneratePodzolPatch(sourceX + 2, sourceY, sourceZ + 2, world);
  for (int_fast32_t i = 0; i < 5; i++) {
    auto n = static_cast<int_fast32_t>(random.NextInt(64));
    if (n % 8 == 0 || n % 8 == 7 || n / 8 == 0 || n / 8 == 7) {
      GeneratePodzolPatch(sourceX - 3 + n % 8, sourceY, sourceZ - 3 + n / 8, world);
    }
  }
}

void MegaPineTree::GeneratePodzolPatch(int_fast32_t sourceX, int_fast32_t sourceY, int_fast32_t sourceZ, ChunkManager &world) {
  for (int_fast32_t x = -2; x <= 2; x++) {
    for (int_fast32_t z = -2; z <= 2; z++) {
      if (abs(x) == 2 && abs(z) == 2) {
        continue;
      }
      for (int_fast32_t y = 2; y >= -3; y--) {
        const MinecraftBlock &block = world.GetBlockAt(sourceX + x, sourceY + y, sourceZ + z);
        if (block == GRASS || block == DIRT) {
          MinecraftBlock dirt = DIRT;
          if (!IS_SOLID(world.GetBlockAt(sourceX + x, sourceY + y + 1, sourceZ + z).GetId())) {
            dirt = PODZOL;
          }

          world.SetBlockAt(sourceX + x, sourceY + y, sourceZ + z, dirt);
        } else if (block != AIR && sourceY + y < sourceY) {
          break;
        }
      }
    }
  }
}
