#ifndef EXT_VANILLAGENERATOR_LIB_GENERATOR_POPULATORS_PLAINSPOPULATOR_H_
#define EXT_VANILLAGENERATOR_LIB_GENERATOR_POPULATORS_PLAINSPOPULATOR_H_

#include <lib/noise/octaves/SimplexOctaveGenerator.h>
#include "BiomePopulator.h"

class PlainsPopulator : public BiomePopulator {
 public:
  void InitPopulators() override;

  void OnGroundPopulation(ChunkManager &world, Random &random, int_fast32_t chunkX, int_fast32_t chunkZ) override;

  [[nodiscard]] std::vector<uint_fast8_t> GetBiomes() const override;
 private:
  static const MinecraftBlock flowers[3];
  static const MinecraftBlock tulips[4];

  Random internalRandom = {2345};
  SimplexOctaveGenerator noiseGen = {internalRandom, 1, 0, 0, 0};
};

#endif //EXT_VANILLAGENERATOR_LIB_GENERATOR_POPULATORS_PLAINSPOPULATOR_H_
