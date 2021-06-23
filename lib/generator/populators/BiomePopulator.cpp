#include <lib/pocketmine/BiomeList.h>
#include <lib/pocketmine/BlockList.h>
#include "BiomePopulator.h"

BiomePopulator::BiomePopulator() {
  waterLakeDecorator = new LakeDecorator(WATER, 8);
  lavaLakeDecorator = new LakeDecorator(LAVA, 16, 8);
  orePopulator = new OrePopulator();

  doublePlantDecorator = new DoublePlantDecorator();
  treeDecorator = new TreeDecorator();
  flowerDecorator = new FlowerDecorator();
  tallGrassDecorator = new TallGrassDecorator();
  deadBushDecorator = new DeadBushDecorator();

  inGroundPopulators.emplace_back(waterLakeDecorator);
  inGroundPopulators.emplace_back(lavaLakeDecorator);
  //inGroundPopulators.emplace_back(surfaceCaveDecorator);
  //inGroundPopulators.emplace_back(dungeonPopulator);
  inGroundPopulators.emplace_back(orePopulator);
  //inGroundPopulators.add(sandPatchDecorator);
  //inGroundPopulators.add(clayPatchDecorator);
  //inGroundPopulators.add(gravelPatchDecorator);

  onGroundPopulators.emplace_back(doublePlantDecorator);
  onGroundPopulators.emplace_back(treeDecorator);
  //onGroundPopulators.add(desertWellDecorator);
  onGroundPopulators.emplace_back(flowerDecorator);
  onGroundPopulators.emplace_back(tallGrassDecorator);
  onGroundPopulators.emplace_back(deadBushDecorator);
  //onGroundPopulators.add(brownMushroomDecorator);
  //onGroundPopulators.add(redMushroomDecorator);
  //onGroundPopulators.add(sugarCaneDecorator);
  //onGroundPopulators.add(pumpkinDecorator);
  //onGroundPopulators.add(cactusDecorator);
  //onGroundPopulators.add(flowingWaterDecorator);
  //onGroundPopulators.add(flowingLavaDecorator);
}

void BiomePopulator::clean() {
  inGroundPopulators.clear();
  onGroundPopulators.clear();

  delete waterLakeDecorator;
  delete lavaLakeDecorator;
  delete orePopulator;
  delete doublePlantDecorator;
  delete treeDecorator;
  delete flowerDecorator;
  delete tallGrassDecorator;
  delete deadBushDecorator;
}

void BiomePopulator::initPopulators() {
  waterLakeDecorator->setAmount(1);
  lavaLakeDecorator->setAmount(1);

  // Follows GlowstoneMC's populators object values.
  doublePlantDecorator->setAmount(0);
  treeDecorator->setAmount(INT32_MIN);
  treeDecorator->setTrees({});
  flowerDecorator->setAmount(2);
  flowerDecorator->setFlowers({{2, DANDELION}, {1, POPPY}});
  tallGrassDecorator->setAmount(1);
  deadBushDecorator->setAmount(0);
}

std::vector<uint_fast8_t> BiomePopulator::getBiomes() {
  return ALL_BIOMES;
}

void BiomePopulator::populate(SimpleChunkManager &world, Random &random, int chunkX, int chunkZ) {
  for (Populator *populator : inGroundPopulators) {
    populator->populate(world, random, chunkX, chunkZ);
  }

  for (Populator *populator : onGroundPopulators) {
    populator->populate(world, random, chunkX, chunkZ);
  }
}