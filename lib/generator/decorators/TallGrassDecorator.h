#ifndef EXT_NOISELIB_LIB_GENERATOR_DECORATORS_TALLGRASSDECORATOR_H_
#define EXT_NOISELIB_LIB_GENERATOR_DECORATORS_TALLGRASSDECORATOR_H_

#include "Decorator.h"

class TallGrassDecorator : public Decorator {
 public:
  void SetFernDensity(double density);

  void Decorate(ChunkManager &world, Random &random, int_fast64_t chunkX, int_fast64_t chunkZ) override;
 private:
  double fern_density_ = 0.0;
};

#endif //EXT_NOISELIB_LIB_GENERATOR_DECORATORS_TALLGRASSDECORATOR_H_
