#ifndef EXT_VANILLAGENERATOR_LIB_GENERATOR_NETHER_POPULATOR_NETHERPOPULATOR_H_
#define EXT_VANILLAGENERATOR_LIB_GENERATOR_NETHER_POPULATOR_NETHERPOPULATOR_H_

#include "NetherBiomePopulator.h"

class NetherPopulator : public Populator {
 public:
  NetherPopulator();

  void RegisterBiomePopulator(NetherBiomePopulator &populator);

  void Populate(ChunkManager &world, Random &random, int_fast32_t chunkX, int_fast32_t chunkZ) override;

 private:
  NetherBiomePopulator defaultPopulator;
};

#endif //EXT_VANILLAGENERATOR_LIB_GENERATOR_NETHER_POPULATOR_NETHERPOPULATOR_H_
