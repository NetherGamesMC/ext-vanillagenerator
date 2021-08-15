#ifndef EXT_VANILLAGENERATOR_LIB_GENERATOR_OVERWORLD_POPULATORS_SWAMPLANDPOPULATOR_H_
#define EXT_VANILLAGENERATOR_LIB_GENERATOR_OVERWORLD_POPULATORS_SWAMPLANDPOPULATOR_H_

#include <lib/generator/overworld/decorators/WaterLilyDecorator.h>
#include <lib/generator/object/trees/defaults/SwampTree.h>
#include "BiomePopulator.h"

class SwamplandPopulator : public BiomePopulator {
 public:
  void InitPopulators() override;

  [[nodiscard]] std::vector<uint_fast8_t> GetBiomes() const override;

  void OnGroundPopulation(ChunkManager &world, Random &random, int_fast32_t chunkX, int_fast32_t chunkZ) override;
 private:
  MushroomDecorator swamplandBrownMushroomDecorator = {BROWN_MUSHROOM};
  MushroomDecorator swamplandRedMushroomDecorator = {RED_MUSHROOM};

  WaterLilyDecorator waterLilyDecorator;

  SwampTree swampTree;
};

#endif //EXT_VANILLAGENERATOR_LIB_GENERATOR_OVERWORLD_POPULATORS_SWAMPLANDPOPULATOR_H_
