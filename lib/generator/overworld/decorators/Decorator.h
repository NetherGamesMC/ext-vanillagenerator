#ifndef EXT_NOISELIB_LIB_GENERATOR_DECORATORS_DECORATOR_H_
#define EXT_NOISELIB_LIB_GENERATOR_DECORATORS_DECORATOR_H_

#include <lib/generator/Populator.h>

class Decorator : public Populator {
 public:
  void SetAmount(int amount);

  virtual void Decorate(ChunkManager &world, Random &random, int_fast32_t chunkX, int_fast32_t chunkZ) = 0;

  void Populate(ChunkManager &world, Random &random, int_fast32_t chunkX, int_fast32_t chunkZ) override;

 protected:
  int amount_ = 0;
};

#endif // EXT_NOISELIB_LIB_GENERATOR_DECORATORS_DECORATOR_H_
