#ifndef EXT_NOISELIB_LIB_GENERATOR_POPULATORS_POPULATOR_H_
#define EXT_NOISELIB_LIB_GENERATOR_POPULATORS_POPULATOR_H_

#include <lib/chunk/SimpleChunkManager.h>
#include <lib/random/Random.h>

class Populator {
 public:
  virtual void Populate(SimpleChunkManager &world, Random &random, int_fast64_t chunkX, int_fast64_t chunkZ) = 0;

  virtual void Clean() {};
};

#endif // EXT_NOISELIB_LIB_GENERATOR_POPULATORS_POPULATOR_H_
