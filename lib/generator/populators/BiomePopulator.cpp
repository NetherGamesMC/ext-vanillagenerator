#include <lib/objects/constants/BiomeList.h>
#include <lib/generator/decorators/SugarCaneDecorator.h>
#include "BiomePopulator.h"

BiomePopulator::BiomePopulator() {
  waterLakeDecorator_ = new LakeDecorator(WATER, 8);
  lavaLakeDecorator_ = new LakeDecorator(LAVA, 16, 8);
  orePopulator_ = new OrePopulator();

  doublePlantDecorator_ = new DoublePlantDecorator();
  treeDecorator_ = new TreeDecorator();
  flowerDecorator_ = new FlowerDecorator();
  tallGrassDecorator_ = new TallGrassDecorator();
  deadBushDecorator_ = new DeadBushDecorator();
  brownMushroomDecorator_ = new MushroomDecorator(BROWN_MUSHROOM);
  redMushroomDecorator_ = new MushroomDecorator(RED_MUSHROOM);
  //sugar_cane_decorator_ = new SugarCaneDecorator();

  inGroundPopulators_.emplace_back(waterLakeDecorator_);
  inGroundPopulators_.emplace_back(lavaLakeDecorator_);
  //inGroundPopulators.emplace_back(surfaceCaveDecorator);
  //inGroundPopulators.emplace_back(dungeonPopulator);
  inGroundPopulators_.emplace_back(orePopulator_);
  //inGroundPopulators.add(sandPatchDecorator);
  //inGroundPopulators.add(clayPatchDecorator);
  //inGroundPopulators.add(gravelPatchDecorator);

  onGroundPopulators_.emplace_back(doublePlantDecorator_);
  onGroundPopulators_.emplace_back(treeDecorator_);
  //onGroundPopulators.add(desertWellDecorator);
  onGroundPopulators_.emplace_back(flowerDecorator_);
  onGroundPopulators_.emplace_back(tallGrassDecorator_);
  onGroundPopulators_.emplace_back(deadBushDecorator_);
  onGroundPopulators_.emplace_back(brownMushroomDecorator_);
  onGroundPopulators_.emplace_back(redMushroomDecorator_);
  //onGroundPopulators.add(sugarCaneDecorator);
  //onGroundPopulators.add(pumpkinDecorator);
  //onGroundPopulators.add(cactusDecorator);
  //onGroundPopulators.add(flowingWaterDecorator);
  //onGroundPopulators.add(flowingLavaDecorator);
}

BiomePopulator::~BiomePopulator() {
  inGroundPopulators_.clear();
  onGroundPopulators_.clear();

  delete waterLakeDecorator_;
  delete lavaLakeDecorator_;
  delete orePopulator_;
  delete doublePlantDecorator_;
  delete treeDecorator_;
  delete flowerDecorator_;
  delete tallGrassDecorator_;
  delete deadBushDecorator_;
  delete brownMushroomDecorator_;
  delete redMushroomDecorator_;
}

void BiomePopulator::InitPopulators() {
  waterLakeDecorator_->SetAmount(1);
  lavaLakeDecorator_->SetAmount(1);

  // Follows GlowstoneMC's populators object values.
  doublePlantDecorator_->SetAmount(0);
  treeDecorator_->SetAmount(INT32_MIN);
  treeDecorator_->setTrees({});
  flowerDecorator_->SetAmount(2);
  flowerDecorator_->SetFlowers({{2, DANDELION}, {1, POPPY}});
  tallGrassDecorator_->SetAmount(1);
  deadBushDecorator_->SetAmount(0);
  brownMushroomDecorator_->SetAmount(1);
  brownMushroomDecorator_->SetDensity(0.25);
  redMushroomDecorator_->SetAmount(1);
  redMushroomDecorator_->SetDensity(0.125);
}

std::vector<uint_fast8_t> BiomePopulator::GetBiomes() {
  return ALL_BIOMES;
}

void BiomePopulator::Populate(ChunkManager &world, Random &random, int_fast64_t chunkX, int_fast64_t chunkZ) {
  for (Populator *populator : inGroundPopulators_) {
    populator->Populate(world, random, chunkX, chunkZ);
  }

  for (Populator *populator : onGroundPopulators_) {
    populator->Populate(world, random, chunkX, chunkZ);
  }
}
