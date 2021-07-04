#ifndef EXT_NOISELIB_LIB_GENERATOR_OVERWORLDPOPULATORS_H_
#define EXT_NOISELIB_LIB_GENERATOR_OVERWORLDPOPULATORS_H_

#include <lib/generator/populators/BiomePopulator.h>

class OverworldPopulator : public Populator {
 public:
  OverworldPopulator();

  ~OverworldPopulator();

  void RegisterBiomePopulator(std::unique_ptr<BiomePopulator> populator);

  void Populate(SimpleChunkManager &world, Random &random, int_fast64_t chunkX, int_fast64_t chunkZ) override;

 private:
  std::map<uint_fast8_t, std::unique_ptr<BiomePopulator>> biomePopulators;
};

#endif // EXT_NOISELIB_LIB_GENERATOR_OVERWORLDPOPULATORS_H_
