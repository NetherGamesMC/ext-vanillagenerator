#include <lib/objects/constants/BiomeList.h>
#include <lib/generator/object/DoubleTallPlant.h>
#include "ForestPopulator.h"

const MinecraftBlock ForestPopulator::blocks[3] = {LILAC, ROSE_BUSH, PEONY};

void ForestPopulator::InitPopulators() {
  doublePlantDecorator_.SetAmount(0);
  treeDecorator_.SetAmount(10);
  treeDecorator_.SetTrees({{4, genericTree}, {1, birchTree}});
  tallGrassDecorator_.SetAmount(2);
}

void ForestPopulator::OnGroundPopulation(ChunkManager &world, Random &random, int_fast32_t chunkX, int_fast32_t chunkZ) {
  int_fast32_t sourceX = chunkX << 4;
  int_fast32_t sourceZ = chunkZ << 4;
  int_fast32_t amount = static_cast<int_fast32_t>(random.NextInt(5)) - doublePlantLoweringAmount;
  int_fast32_t i = 0;
  while (i < amount) {
    for (int_fast32_t j = 0; j < 5; j++, i++) {
      int_fast32_t x = sourceX + static_cast<int_fast32_t>(random.NextInt(16));
      int_fast32_t z = sourceZ + static_cast<int_fast32_t>(random.NextInt(16));
      int_fast32_t y = static_cast<int_fast32_t>(random.NextInt(world.GetHighestElevationAt(x, z) + 32));
      MinecraftBlock species = blocks[random.NextInt(2)];
      if (DoubleTallPlant(species).Generate(world, random, x, y, z)) {
        i++;
        break;
      }
    }
  }

  BiomePopulator::OnGroundPopulation(world, random, chunkX, chunkZ);
}

std::vector<uint_fast8_t> ForestPopulator::GetBiomes() const {
  return {FOREST, FOREST_HILLS};
}
