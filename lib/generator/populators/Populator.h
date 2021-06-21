#ifndef EXT_NOISELIB_LIB_GENERATOR_POPULATORS_POPULATOR_H_
#define EXT_NOISELIB_LIB_GENERATOR_POPULATORS_POPULATOR_H_

#include <lib/chunk/SimpleChunkManager.h>
#include <lib/random/Random.h>

class Populator {
 public:
  virtual void populate(SimpleChunkManager &world, Random &random, int chunkX, int chunkZ) = 0;
};

#endif // EXT_NOISELIB_LIB_GENERATOR_POPULATORS_POPULATOR_H_
