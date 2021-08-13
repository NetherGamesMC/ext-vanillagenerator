#include <lib/objects/constants/BiomeList.h>
#include "BiomePopulator.h"

BiomePopulator::BiomePopulator() {
  BiomePopulator::InitPopulators();
}

void BiomePopulator::InitPopulators() {
  waterLakeDecorator_.SetAmount(1);
  lavaLakeDecorator_.SetAmount(1);
  sandPatchDecorator_.SetAmount(3);
  sandPatchDecorator_.SetRadii(7, 2);
  sandPatchDecorator_.SetOverridableBlocks({DIRT, GRASS});
  clayPatchDecorator_.SetAmount(1);
  clayPatchDecorator_.SetRadii(4, 1);
  clayPatchDecorator_.SetOverridableBlocks({DIRT});
  gravelPatchDecorator_.SetAmount(1);
  gravelPatchDecorator_.SetRadii(6, 2);
  gravelPatchDecorator_.SetOverridableBlocks({DIRT, GRASS});

  // Follows GlowstoneMC's populators object values.
  doublePlantDecorator_.SetAmount(0);
  treeDecorator_.SetAmount(INT32_MIN);
  treeDecorator_.SetTrees({{1, bigOakTree}, {9, genericTree}});
  flowerDecorator_.SetAmount(2);
  flowerDecorator_.SetFlowers({{2, DANDELION}, {1, POPPY}});
  tallGrassDecorator_.SetAmount(1);
  deadBushDecorator_.SetAmount(0);
  brownMushroomDecorator_.SetAmount(1);
  brownMushroomDecorator_.SetDensity(0.25);
  redMushroomDecorator_.SetAmount(1);
  redMushroomDecorator_.SetDensity(0.125);
  sugarCaneDecorator_.SetAmount(10);
  cactusDecorator_.SetAmount(0);
}

std::vector<uint_fast8_t> BiomePopulator::GetBiomes() const {
  return ALL_BIOMES;
}

void BiomePopulator::Populate(ChunkManager &world, Random &random, int_fast32_t chunkX, int_fast32_t chunkZ) {
  waterLakeDecorator_.Populate(world, random, chunkX, chunkZ);
  lavaLakeDecorator_.Populate(world, random, chunkX, chunkZ);
  orePopulator_.Populate(world, random, chunkX, chunkZ);
  sandPatchDecorator_.Populate(world, random, chunkX, chunkZ);
  clayPatchDecorator_.Populate(world, random, chunkX, chunkZ);
  gravelPatchDecorator_.Populate(world, random, chunkX, chunkZ);

  OnGroundPopulation(world, random, chunkX, chunkZ);
}

void BiomePopulator::OnGroundPopulation(ChunkManager &world, Random &random, int_fast32_t chunkX, int_fast32_t chunkZ) {
  doublePlantDecorator_.Populate(world, random, chunkX, chunkZ);
  treeDecorator_.Populate(world, random, chunkX, chunkZ);
  flowerDecorator_.Populate(world, random, chunkX, chunkZ);
  tallGrassDecorator_.Populate(world, random, chunkX, chunkZ);
  deadBushDecorator_.Populate(world, random, chunkX, chunkZ);
  brownMushroomDecorator_.Populate(world, random, chunkX, chunkZ);
  redMushroomDecorator_.Populate(world, random, chunkX, chunkZ);
  sugarCaneDecorator_.Populate(world, random, chunkX, chunkZ);
  pumpkinDecorator_.Populate(world, random, chunkX, chunkZ);
  cactusDecorator_.Populate(world, random, chunkX, chunkZ);
}
