#ifndef EXT_VANILLAGENERATOR_LIB_GENERATOR_DECORATORS_MUSHROOMDECORATOR_H_
#define EXT_VANILLAGENERATOR_LIB_GENERATOR_DECORATORS_MUSHROOMDECORATOR_H_

#include "Decorator.h"

using namespace blocks;

class MushroomDecorator : public Decorator {
 public:
  MushroomDecorator(const MCBlock *block) : block_(block) {}

  void SetUseFixedHeightRange();

  void SetDensity(double density);

  void Decorate(ChunkManager &world, Random &random, int_fast32_t chunkX, int_fast32_t chunkZ) override;

 private:
  const MCBlock *block_;

  double density_ = 0.0;
  bool fixedHeightRange_ = false;
};

#endif //EXT_VANILLAGENERATOR_LIB_GENERATOR_DECORATORS_MUSHROOMDECORATOR_H_
