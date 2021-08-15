#ifndef EXT_VANILLAGENERATOR_LIB_GENERATOR_POPULATORS_BIOMEPOPULATOR_H_
#define EXT_VANILLAGENERATOR_LIB_GENERATOR_POPULATORS_BIOMEPOPULATOR_H_

#include <lib/objects/random/Random.h>
#include <lib/generator/object/trees/defaults/DarkOakTree.h>
#include <lib/generator/overworld/decorators/LakeDecorator.h>
#include <lib/generator/overworld/decorators/DoublePlantDecorator.h>
#include <lib/generator/overworld/decorators/TreeDecorator.h>
#include <lib/generator/overworld/decorators/FlowerDecorator.h>
#include <lib/generator/overworld/decorators/TallGrassDecorator.h>
#include <lib/generator/overworld/decorators/DeadBushDecorator.h>
#include <lib/generator/overworld/decorators/MushroomDecorator.h>
#include <lib/generator/overworld/decorators/SugarCaneDecorator.h>
#include <lib/generator/overworld/decorators/PumpkinDecorator.h>
#include <lib/generator/overworld/decorators/CactusDecorator.h>
#include <lib/generator/overworld/decorators/UnderwaterDecorator.h>
#include <lib/generator/object/trees/defaults/JungleTree.h>
#include <lib/generator/object/trees/defaults/BigOakTree.h>
#include <lib/generator/object/trees/defaults/AcaciaTree.h>
#include <lib/generator/object/trees/defaults/BrownMushroomTree.h>
#include <lib/generator/object/trees/defaults/CocoaTree.h>
#include "lib/generator/Populator.h"
#include "OrePopulator.h"

class BiomePopulator : public Populator {
 public:
  BiomePopulator();

  void Populate(ChunkManager &world, Random &random, int_fast32_t chunkX, int_fast32_t chunkZ) override;

  virtual void OnGroundPopulation(ChunkManager &world, Random &random, int_fast32_t chunkX, int_fast32_t chunkZ);
  virtual void InitPopulators();

  [[nodiscard]] virtual std::vector<uint_fast8_t> GetBiomes() const;
 protected:
  BigOakTree bigOakTree;
  GenericTree genericTree;

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

#endif //EXT_VANILLAGENERATOR_LIB_GENERATOR_POPULATORS_BIOMEPOPULATOR_H_
