#ifndef EXT_NOISELIB_LIB_GENERATOR_POPULATORS_BIOMEPOPULATOR_H_
#define EXT_NOISELIB_LIB_GENERATOR_POPULATORS_BIOMEPOPULATOR_H_

#include <lib/objects/random/Random.h>
#include <lib/generator/objects/trees/defaults/DarkOakTree.h>
#include <lib/generator/decorators/LakeDecorator.h>
#include <lib/generator/decorators/DoublePlantDecorator.h>
#include <lib/generator/decorators/TreeDecorator.h>
#include <lib/generator/decorators/FlowerDecorator.h>
#include <lib/generator/decorators/TallGrassDecorator.h>
#include <lib/generator/decorators/DeadBushDecorator.h>
#include <lib/generator/decorators/MushroomDecorator.h>
#include <lib/generator/decorators/SugarCaneDecorator.h>
#include <lib/generator/decorators/PumpkinDecorator.h>
#include <lib/generator/decorators/CactusDecorator.h>
#include <lib/generator/decorators/UnderwaterDecorator.h>
#include <lib/generator/objects/trees/defaults/JungleTree.h>
#include "Populator.h"
#include "OrePopulator.h"

class BiomePopulator : public Populator {
 public:
  BiomePopulator();

  void Populate(ChunkManager &world, Random &random, int_fast32_t chunkX, int_fast32_t chunkZ) override;

  virtual std::vector<uint_fast8_t> GetBiomes();

  virtual void InitPopulators();

 private:
  DarkOakTree darkOakTree_;
  JungleTree jungleTree_;

  LakeDecorator waterLakeDecorator_ = {WATER, 4};
  LakeDecorator lavaLakeDecorator_ = {LAVA, 8, 8};
  OrePopulator orePopulator_;
  UnderwaterDecorator sandPatchDecorator_ = {SAND};
  UnderwaterDecorator clayPatchDecorator_ = {CLAY};
  UnderwaterDecorator gravelPatchDecorator_ = {GRAVEL};

  DoublePlantDecorator doublePlantDecorator_;
  TreeDecorator treeDecorator_;
  FlowerDecorator flowerDecorator_;
  TallGrassDecorator tallGrassDecorator_;
  DeadBushDecorator deadBushDecorator_;
  MushroomDecorator brownMushroomDecorator_ = {BROWN_MUSHROOM};
  MushroomDecorator redMushroomDecorator_ = {RED_MUSHROOM};
  SugarCaneDecorator sugarCaneDecorator_;
  PumpkinDecorator pumpkinDecorator_;
  CactusDecorator cactusDecorator_;
};

#endif //EXT_NOISELIB_LIB_GENERATOR_POPULATORS_BIOMEPOPULATOR_H_
