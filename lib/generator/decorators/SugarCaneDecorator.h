#ifndef EXT_NOISELIB_LIB_GENERATOR_DECORATORS_SUGARCANEDECORATOR_H_
#define EXT_NOISELIB_LIB_GENERATOR_DECORATORS_SUGARCANEDECORATOR_H_

#include "Decorator.h"

class SugarCaneDecorator : public Decorator {
  void Decorate(SimpleChunkManager &world, Random &random, int_fast64_t chunkX, int_fast64_t chunkZ);
};

#endif //EXT_NOISELIB_LIB_GENERATOR_DECORATORS_SUGARCANEDECORATOR_H_
