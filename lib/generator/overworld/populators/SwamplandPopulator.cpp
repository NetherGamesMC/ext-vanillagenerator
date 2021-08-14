#include <lib/objects/constants/BiomeList.h>
#include "SwamplandPopulator.h"

void SwamplandPopulator::InitPopulators() {
  sandPatchDecorator_.SetAmount(0);
  gravelPatchDecorator_.SetAmount(2);
  treeDecorator_.SetAmount(2);
  treeDecorator_.SetTrees({{1, swampTree}});
  flowerDecorator_.SetAmount(1);
  flowerDecorator_.SetFlowers({{1, BLUE_ORCHID}});
  tallGrassDecorator_.SetAmount(5);
  sugarCaneDecorator_.SetAmount(20);
  swamplandBrownMushroomDecorator.SetAmount(8);
  swamplandBrownMushroomDecorator.SetDensity(0.01);
  swamplandBrownMushroomDecorator.SetUseFixedHeightRange();
  swamplandRedMushroomDecorator.SetAmount(8);
  swamplandRedMushroomDecorator.SetDensity(0.01);
  swamplandRedMushroomDecorator.SetUseFixedHeightRange();
  waterLilyDecorator.SetAmount(4);
}

std::vector<uint_fast8_t> SwamplandPopulator::GetBiomes() const {
  return {SWAMPLAND, SWAMPLAND_MOUNTAINS};
}

void SwamplandPopulator::OnGroundPopulation(ChunkManager &world, Random &random, int_fast32_t chunkX, int_fast32_t chunkZ) {
  BiomePopulator::OnGroundPopulation(world, random, chunkX, chunkZ);

  swamplandBrownMushroomDecorator.Populate(world, random, chunkX, chunkZ);
  swamplandRedMushroomDecorator.Populate(world, random, chunkX, chunkZ);
  waterLilyDecorator.Populate(world, random, chunkX, chunkZ);
}
