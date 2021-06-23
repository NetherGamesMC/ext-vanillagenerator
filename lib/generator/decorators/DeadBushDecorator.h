#ifndef EXT_NOISELIB_LIB_GENERATOR_DECORATORS_DEADBUSHDECORATOR_H_
#define EXT_NOISELIB_LIB_GENERATOR_DECORATORS_DEADBUSHDECORATOR_H_

#include "Decorator.h"

class DeadBushDecorator : public Decorator{
 public:
  void decorate(SimpleChunkManager &world, Random &random, int chunkX, int chunkZ) override;
};

#endif //EXT_NOISELIB_LIB_GENERATOR_DECORATORS_DEADBUSHDECORATOR_H_
