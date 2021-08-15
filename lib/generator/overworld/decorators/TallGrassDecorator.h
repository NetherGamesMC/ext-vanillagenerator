#ifndef EXT_VANILLAGENERATOR_LIB_GENERATOR_DECORATORS_TALLGRASSDECORATOR_H_
#define EXT_VANILLAGENERATOR_LIB_GENERATOR_DECORATORS_TALLGRASSDECORATOR_H_

#include "Decorator.h"

class TallGrassDecorator : public Decorator {
 public:
  void SetFernDensity(double density);

  void Decorate(ChunkManager &world, Random &random, int_fast32_t chunkX, int_fast32_t chunkZ) override;
 private:
  double fernDensity_ = 0.0;
};

#endif //EXT_VANILLAGENERATOR_LIB_GENERATOR_DECORATORS_TALLGRASSDECORATOR_H_
