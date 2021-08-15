#ifndef EXT_VANILLAGENERATOR_LIB_GENERATOR_POPULATORS_FLOWERFORESTPOPULATOR_H_
#define EXT_VANILLAGENERATOR_LIB_GENERATOR_POPULATORS_FLOWERFORESTPOPULATOR_H_

#include <lib/noise/octaves/SimplexOctaveGenerator.h>
#include "ForestPopulator.h"

class FlowerForestPopulator : public ForestPopulator {
 public:
  void InitPopulators() override;

  void OnGroundPopulation(ChunkManager &world, Random &random, int_fast32_t chunkX, int_fast32_t chunkZ) override;

  [[nodiscard]] std::vector<uint_fast8_t> GetBiomes() const override;
 private:
  Random internalRandom = {2345};
  SimplexOctaveGenerator noiseGen = {internalRandom, 1, 0, 0, 0};

  static const MinecraftBlock flowers[10];
};

#endif //EXT_VANILLAGENERATOR_LIB_GENERATOR_POPULATORS_FLOWERFORESTPOPULATOR_H_
