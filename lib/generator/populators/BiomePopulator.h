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
  std::vector<Populator *> in_ground_populators_;
  std::vector<Populator *> on_ground_populators_;

  LakeDecorator *water_lake_decorator_;
  LakeDecorator *lava_lake_decorator_;
  OrePopulator *ore_populator_;

  DoublePlantDecorator *double_plant_decorator_;
  TreeDecorator *tree_decorator_;
  FlowerDecorator *flower_decorator_;
  TallGrassDecorator *tall_grass_decorator_;
  DeadBushDecorator *dead_bush_decorator_;
  MushroomDecorator *brown_mushroom_decorator_;
  MushroomDecorator *red_mushroom_decorator_;
};

#endif //EXT_NOISELIB_LIB_GENERATOR_POPULATORS_BIOMEPOPULATOR_H_
