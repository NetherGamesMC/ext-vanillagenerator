#ifndef EXT_NOISELIB_LIB_GENERATOR_POPULATORS_BIOMEPOPULATOR_H_
#define EXT_NOISELIB_LIB_GENERATOR_POPULATORS_BIOMEPOPULATOR_H_

#include <lib/objects/random/Random.h>
#include <lib/generator/decorators/LakeDecorator.h>
#include <lib/generator/decorators/DoublePlantDecorator.h>
#include <lib/generator/decorators/TreeDecorator.h>
#include <lib/generator/decorators/FlowerDecorator.h>
#include <lib/generator/decorators/TallGrassDecorator.h>
#include <lib/generator/decorators/DeadBushDecorator.h>
#include <lib/generator/decorators/MushroomDecorator.h>
#include "Populator.h"
#include "OrePopulator.h"

class BiomePopulator : public Populator {
 public:
  BiomePopulator();

  ~BiomePopulator();

  void Populate(ChunkManager &world, Random &random, int_fast64_t chunkX, int_fast64_t chunkZ) override;

  virtual std::vector<uint_fast8_t> GetBiomes();

  virtual void InitPopulators();

 private:
  std::vector<Populator *> inGroundPopulators_;
  std::vector<Populator *> onGroundPopulators_;

  LakeDecorator *waterLakeDecorator_;
  LakeDecorator *lavaLakeDecorator_;
  OrePopulator *orePopulator_;

  DoublePlantDecorator *doublePlantDecorator_;
  TreeDecorator *treeDecorator_;
  FlowerDecorator *flowerDecorator_;
  TallGrassDecorator *tallGrassDecorator_;
  DeadBushDecorator *deadBushDecorator_;
  MushroomDecorator *brownMushroomDecorator_;
  MushroomDecorator *redMushroomDecorator_;
};

#endif //EXT_NOISELIB_LIB_GENERATOR_POPULATORS_BIOMEPOPULATOR_H_
