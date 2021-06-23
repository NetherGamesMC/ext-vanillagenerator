#ifndef EXT_NOISELIB_LIB_GENERATOR_DECORATORS_TALLGRASSDECORATOR_H_
#define EXT_NOISELIB_LIB_GENERATOR_DECORATORS_TALLGRASSDECORATOR_H_

#include "Decorator.h"

class TallGrassDecorator : public Decorator {
 public:
  void setFernDensity(double fernIntensity);

  void decorate(SimpleChunkManager &world, Random &random, int chunkX, int chunkZ) override;
 private:
  double fernIntensity_ = 0.0;
};

#endif //EXT_NOISELIB_LIB_GENERATOR_DECORATORS_TALLGRASSDECORATOR_H_
