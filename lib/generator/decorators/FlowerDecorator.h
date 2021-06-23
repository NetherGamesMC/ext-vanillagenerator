#ifndef EXT_NOISELIB_LIB_GENERATOR_DECORATORS_FLOWERDECORATOR_H_
#define EXT_NOISELIB_LIB_GENERATOR_DECORATORS_FLOWERDECORATOR_H_

#include "Decorator.h"

struct FlowerDecoration {
  int weight;
  MinecraftBlock block;
};

class FlowerDecorator : public Decorator {
 public:
  void setFlowers(std::vector<FlowerDecoration> flowers);

  void decorate(SimpleChunkManager &world, Random &random, int chunkX, int chunkZ) override;
 private:
  MinecraftBlock getRandomFlower(Random random);

  std::vector<FlowerDecoration> decorations;
};

#endif // EXT_NOISELIB_LIB_GENERATOR_DECORATORS_FLOWERDECORATOR_H_
