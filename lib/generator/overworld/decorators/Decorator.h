#ifndef EXT_VANILLAGENERATOR_LIB_GENERATOR_DECORATORS_DECORATOR_H_
#define EXT_VANILLAGENERATOR_LIB_GENERATOR_DECORATORS_DECORATOR_H_

#include <lib/generator/Populator.h>

class Decorator : public Populator {
 public:
  void SetAmount(int_fast32_t amount);

  virtual void Decorate(ChunkManager &world, Random &random, int_fast32_t chunkX, int_fast32_t chunkZ) = 0;

  void Populate(ChunkManager &world, Random &random, int_fast32_t chunkX, int_fast32_t chunkZ) override;

 protected:
  int_fast32_t amount_ = 0;
};

#endif // EXT_VANILLAGENERATOR_LIB_GENERATOR_DECORATORS_DECORATOR_H_
