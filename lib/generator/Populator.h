#ifndef EXT_VANILLAGENERATOR_LIB_GENERATOR_POPULATORS_POPULATOR_H_
#define EXT_VANILLAGENERATOR_LIB_GENERATOR_POPULATORS_POPULATOR_H_

#include <lib/chunk/ChunkManager.h>
#include <lib/objects/random/Random.h>

class Populator {
 public:
  virtual void Populate(ChunkManager &world, Random &random, int_fast32_t chunkX, int_fast32_t chunkZ) = 0;
};

#endif // EXT_VANILLAGENERATOR_LIB_GENERATOR_POPULATORS_POPULATOR_H_
