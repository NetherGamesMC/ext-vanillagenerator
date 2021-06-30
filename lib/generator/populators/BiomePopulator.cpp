#include <lib/pocketmine/BiomeList.h>
#include <lib/pocketmine/BlockList.h>
#include "BiomePopulator.h"

BiomePopulator::BiomePopulator() {
  water_lake_decorator_ = new LakeDecorator(WATER, 8);
  lava_lake_decorator_ = new LakeDecorator(LAVA, 16, 8);
  ore_populator_ = new OrePopulator();

  double_plant_decorator_ = new DoublePlantDecorator();
  tree_decorator_ = new TreeDecorator();
  flower_decorator_ = new FlowerDecorator();
  tall_grass_decorator_ = new TallGrassDecorator();
  dead_bush_decorator_ = new DeadBushDecorator();
  brown_mushroom_decorator_ = new MushroomDecorator(BROWN_MUSHROOM);
  red_mushroom_decorator_ = new MushroomDecorator(RED_MUSHROOM);

  in_ground_populators_.emplace_back(water_lake_decorator_);
  in_ground_populators_.emplace_back(lava_lake_decorator_);
  //inGroundPopulators.emplace_back(surfaceCaveDecorator);
  //inGroundPopulators.emplace_back(dungeonPopulator);
  in_ground_populators_.emplace_back(ore_populator_);
  //inGroundPopulators.add(sandPatchDecorator);
  //inGroundPopulators.add(clayPatchDecorator);
  //inGroundPopulators.add(gravelPatchDecorator);

  on_ground_populators_.emplace_back(double_plant_decorator_);
  on_ground_populators_.emplace_back(tree_decorator_);
  //onGroundPopulators.add(desertWellDecorator);
  on_ground_populators_.emplace_back(flower_decorator_);
  on_ground_populators_.emplace_back(tall_grass_decorator_);
  on_ground_populators_.emplace_back(dead_bush_decorator_);
  on_ground_populators_.emplace_back(brown_mushroom_decorator_);
  on_ground_populators_.emplace_back(red_mushroom_decorator_);
  //onGroundPopulators.add(redMushroomDecorator);
  //onGroundPopulators.add(sugarCaneDecorator);
  //onGroundPopulators.add(pumpkinDecorator);
  //onGroundPopulators.add(cactusDecorator);
  //onGroundPopulators.add(flowingWaterDecorator);
  //onGroundPopulators.add(flowingLavaDecorator);
}

void BiomePopulator::Clean() {
  in_ground_populators_.clear();
  on_ground_populators_.clear();

  delete water_lake_decorator_;
  delete lava_lake_decorator_;
  delete ore_populator_;
  delete double_plant_decorator_;
  delete tree_decorator_;
  delete flower_decorator_;
  delete tall_grass_decorator_;
  delete dead_bush_decorator_;
  delete brown_mushroom_decorator_;
  delete red_mushroom_decorator_;
}

void BiomePopulator::InitPopulators() {
  water_lake_decorator_->SetAmount(1);
  lava_lake_decorator_->SetAmount(1);

  // Follows GlowstoneMC's populators object values.
  double_plant_decorator_->SetAmount(0);
  tree_decorator_->SetAmount(INT32_MIN);
  tree_decorator_->setTrees({});
  flower_decorator_->SetAmount(2);
  flower_decorator_->SetFlowers({{2, DANDELION}, {1, POPPY}});
  tall_grass_decorator_->SetAmount(1);
  dead_bush_decorator_->SetAmount(0);
  brown_mushroom_decorator_->SetAmount(1);
  brown_mushroom_decorator_->SetDensity(0.25);
  red_mushroom_decorator_->SetAmount(1);
  red_mushroom_decorator_->SetDensity(0.125);
}

std::vector<uint_fast8_t> BiomePopulator::GetBiomes() {
  return ALL_BIOMES;
}

void BiomePopulator::Populate(SimpleChunkManager &world, Random &random, int_fast64_t chunkX, int_fast64_t chunkZ) {
  for (Populator *populator : in_ground_populators_) {
    populator->Populate(world, random, chunkX, chunkZ);
  }

  for (Populator *populator : on_ground_populators_) {
    populator->Populate(world, random, chunkX, chunkZ);
  }
}