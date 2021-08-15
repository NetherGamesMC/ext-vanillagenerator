#ifndef EXT_VANILLAGENERATOR_LIB_GENERATOR_POPULATORS_FORESTPOPULATOR_H_
#define EXT_VANILLAGENERATOR_LIB_GENERATOR_POPULATORS_FORESTPOPULATOR_H_

#include <lib/generator/object/trees/defaults/BirchTree.h>
#include "BiomePopulator.h"

class ForestPopulator : public BiomePopulator {
 public:
  void InitPopulators() override;

  void OnGroundPopulation(ChunkManager &world, Random &random, int_fast32_t chunkX, int_fast32_t chunkZ) override;

  [[nodiscard]] std::vector<uint_fast8_t> GetBiomes() const override;
 protected:
  GenericTree genericTree;
  BirchTree birchTree;

  int_fast32_t doublePlantLoweringAmount = 3;

  const static MinecraftBlock blocks[3];
};

#endif //EXT_VANILLAGENERATOR_LIB_GENERATOR_POPULATORS_FORESTPOPULATOR_H_
