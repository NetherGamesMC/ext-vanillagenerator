#include <lib/objects/constants/BiomeList.h>
#include "JunglePopulator.h"

void JunglePopulator::InitPopulators() {
  treeDecorator_.SetAmount(65);
  treeDecorator_.SetTrees({{10, bigOakTree}, {50, jungleBush}, {15, megaJungleTree}, {30, cocoaTree}});
  flowerDecorator_.SetAmount(4);
  tallGrassDecorator_.SetAmount(25);
  tallGrassDecorator_.SetFernDensity(0.25);
}

std::vector<uint_fast8_t> JunglePopulator::GetBiomes() const {
  return {JUNGLE, JUNGLE_HILLS, JUNGLE_MOUNTAINS};
}

void JunglePopulator::OnGroundPopulation(ChunkManager &world, Random &random, int_fast32_t chunkX, int_fast32_t chunkZ) {
  int_fast32_t sourceX = chunkX << 4;
  int_fast32_t sourceZ = chunkZ << 4;

  for (int_fast32_t i = 0; i < 7; i++) {
    int_fast32_t x = sourceX + random.NextInt(16);
    int_fast32_t z = sourceZ + random.NextInt(16);
    int_fast32_t y = world.GetHighestElevationAt(x, z);

    BlockTransaction txn = BlockTransaction(world);

    JungleBush bushTree = JungleBush();
    bushTree.Initialize(random, txn);

    if (bushTree.Generate(world, random, x, y, z)) {
      txn.ApplyBlockChanges();
    }
  }

  BiomePopulator::OnGroundPopulation(world, random, chunkX, chunkZ);
}


