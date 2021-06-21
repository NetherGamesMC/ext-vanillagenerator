#ifndef EXT_NOISELIB_LIB_GENERATOR_OVERWORLDPOPULATORS_H_
#define EXT_NOISELIB_LIB_GENERATOR_OVERWORLDPOPULATORS_H_

#include <lib/generator/populators/BiomePopulator.h>

class OverworldPopulator {
 public:
  OverworldPopulator();

  void registerBiomePopulator(BiomePopulator *populator);

  void populate(SimpleChunkManager world, Random random, int chunkX, int chunkZ);

  void destroy();

 private:
  std::map<uint_fast8_t, BiomePopulator *> biomePopulators;
};

#endif // EXT_NOISELIB_LIB_GENERATOR_OVERWORLDPOPULATORS_H_
