#ifndef EXT_VANILLAGENERATOR_LIB_GENERATOR_DECORATORS_SUGARCANEDECORATOR_H_
#define EXT_VANILLAGENERATOR_LIB_GENERATOR_DECORATORS_SUGARCANEDECORATOR_H_

#include "Decorator.h"

class SugarCaneDecorator : public Decorator {
 public:
  void Decorate(ChunkManager &world, Random &random, int_fast32_t chunkX, int_fast32_t chunkZ) override;
};

#endif //EXT_VANILLAGENERATOR_LIB_GENERATOR_DECORATORS_SUGARCANEDECORATOR_H_
