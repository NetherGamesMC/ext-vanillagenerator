#ifndef EXT_VANILLAGENERATOR_LIB_GENERATOR_OVERWORLD_DECORATORS_TAIGAPOPULATOR_H_
#define EXT_VANILLAGENERATOR_LIB_GENERATOR_OVERWORLD_DECORATORS_TAIGAPOPULATOR_H_

#include <lib/generator/overworld/populators/BiomePopulator.h>
#include <lib/generator/object/trees/defaults/RedwoodTree.h>
#include <lib/generator/object/trees/defaults/TallRedwoodTree.h>

class TaigaPopulator : public BiomePopulator {
 public:
  void InitPopulators() override;

  [[nodiscard]] std::vector<uint_fast8_t> GetBiomes() const override;

  void OnGroundPopulation(ChunkManager &world, Random &random, int_fast32_t chunkX, int_fast32_t chunkZ) override;
 protected:
  MushroomDecorator taigaBrownMushroomDecorator = {BROWN_MUSHROOM};
  MushroomDecorator taigaRedMushroomDecorator = {RED_MUSHROOM};

  RedwoodTree redwoodTree;
  TallRedwoodTree tallRedwoodTree;
};

#endif //EXT_VANILLAGENERATOR_LIB_GENERATOR_OVERWORLD_DECORATORS_TAIGAPOPULATOR_H_
