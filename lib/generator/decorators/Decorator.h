#ifndef EXT_NOISELIB_LIB_GENERATOR_DECORATORS_DECORATOR_H_
#define EXT_NOISELIB_LIB_GENERATOR_DECORATORS_DECORATOR_H_

#include <lib/generator/populators/Populator.h>

class Decorator : public Populator {
 public:
  void setAmount(int amountVal);

  virtual void decorate(SimpleChunkManager &world, Random &random, int chunkX, int chunkZ) = 0;

  void populate(SimpleChunkManager &world, Random &random, int chunkX, int chunkZ) override;

 protected:
  int amount = 0;
};

#endif // EXT_NOISELIB_LIB_GENERATOR_DECORATORS_DECORATOR_H_
