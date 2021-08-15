#ifndef EXT_VANILLAGENERATOR_LIB_GENERATOR_OVERWORLD_DECORATORS_MELONDECORATOR_H_
#define EXT_VANILLAGENERATOR_LIB_GENERATOR_OVERWORLD_DECORATORS_MELONDECORATOR_H_

#include "Decorator.h"

class MelonDecorator : public Decorator {
 public:
  void Decorate(ChunkManager &world, Random &random, int_fast32_t chunkX, int_fast32_t chunkZ) override;
};

#endif //EXT_VANILLAGENERATOR_LIB_GENERATOR_OVERWORLD_DECORATORS_MELONDECORATOR_H_
