#ifndef EXT_NOISELIB_LIB_GENERATOR_POPULATORS_BIOMEPOPULATOR_H_
#define EXT_NOISELIB_LIB_GENERATOR_POPULATORS_BIOMEPOPULATOR_H_

#include <lib/random/Random.h>
#include <lib/generator/decorators/LakeDecorator.h>
#include <lib/generator/decorators/DoublePlantDecorator.h>
#include <lib/generator/decorators/TreeDecorator.h>
#include "Populator.h"
#include "OrePopulator.h"

class BiomePopulator : public Populator {
 public:
  BiomePopulator();

  void populate(SimpleChunkManager &world, Random &random, int chunkX, int chunkZ) override;

  virtual std::vector<uint_fast8_t> getBiomes();

  virtual void initPopulators();

  void clean();

 private:
  std::vector<Populator *> inGroundPopulators;
  std::vector<Populator *> onGroundPopulators;

  LakeDecorator *waterLakeDecorator;
  LakeDecorator *lavaLakeDecorator;
  OrePopulator *orePopulator;

  DoublePlantDecorator *doublePlantDecorator;
  TreeDecorator *treeDecorator;
};

#endif //EXT_NOISELIB_LIB_GENERATOR_POPULATORS_BIOMEPOPULATOR_H_
