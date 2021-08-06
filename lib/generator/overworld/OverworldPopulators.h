#ifndef EXT_NOISELIB_LIB_GENERATOR_OVERWORLDPOPULATORS_H_
#define EXT_NOISELIB_LIB_GENERATOR_OVERWORLDPOPULATORS_H_

#include <lib/generator/populators/BiomePopulator.h>

class OverworldPopulator : public Populator {
 public:
  OverworldPopulator();

  ~OverworldPopulator();

  void RegisterBiomePopulator(BiomePopulator *populator);

  void Populate(ChunkManager &world, Random &random, int_fast32_t chunkX, int_fast32_t chunkZ) override;

 private:
  std::map<uint_fast8_t, BiomePopulator*> biomePopulators;
};

#endif // EXT_NOISELIB_LIB_GENERATOR_OVERWORLDPOPULATORS_H_
