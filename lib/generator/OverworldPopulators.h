#ifndef EXT_NOISELIB_LIB_GENERATOR_OVERWORLDPOPULATORS_H_
#define EXT_NOISELIB_LIB_GENERATOR_OVERWORLDPOPULATORS_H_

#include <lib/generator/populators/BiomePopulator.h>

class OverworldPopulator : public Populator {
 public:
  OverworldPopulator();

  void RegisterBiomePopulator(BiomePopulator *populator);

  void Populate(SimpleChunkManager &world, Random &random, int_fast64_t chunkX, int_fast64_t chunkZ) override;

  void Destroy();

 private:
  std::map<uint_fast8_t, BiomePopulator *> biomePopulators;
};

#endif // EXT_NOISELIB_LIB_GENERATOR_OVERWORLDPOPULATORS_H_
