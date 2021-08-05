#ifndef EXT_NOISELIB_LIB_GENERATOR_DECORATORS_FLOWERDECORATOR_H_
#define EXT_NOISELIB_LIB_GENERATOR_DECORATORS_FLOWERDECORATOR_H_

#include "Decorator.h"

struct FlowerDecoration {
  int weight;
  MinecraftBlock block;
};

class FlowerDecorator : public Decorator {
 public:
  void SetFlowers(std::vector<FlowerDecoration> decorations);

  void Decorate(ChunkManager &world, Random &random, int_fast64_t chunkX, int_fast64_t chunkZ) override;
 private:
  MinecraftBlock GetRandomFlower(Random random);

  std::vector<FlowerDecoration> decorations_;
};

#endif // EXT_NOISELIB_LIB_GENERATOR_DECORATORS_FLOWERDECORATOR_H_
