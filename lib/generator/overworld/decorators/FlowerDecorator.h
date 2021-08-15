#ifndef EXT_VANILLAGENERATOR_LIB_GENERATOR_DECORATORS_FLOWERDECORATOR_H_
#define EXT_VANILLAGENERATOR_LIB_GENERATOR_DECORATORS_FLOWERDECORATOR_H_

#include "Decorator.h"

struct FlowerDecoration {
  int_fast32_t weight;
  MinecraftBlock block;
};

class FlowerDecorator : public Decorator {
 public:
  void SetFlowers(std::vector<FlowerDecoration> decorations);

  void Decorate(ChunkManager &world, Random &random, int_fast32_t chunkX, int_fast32_t chunkZ) override;
 private:
  MinecraftBlock GetRandomFlower(Random random);

  std::vector<FlowerDecoration> decorations_;
};

#endif // EXT_VANILLAGENERATOR_LIB_GENERATOR_DECORATORS_FLOWERDECORATOR_H_
