#ifndef EXT_NOISELIB_LIB_GENERATOR_DECORATORS_DOUBLEPLANTDECORATOR_H_
#define EXT_NOISELIB_LIB_GENERATOR_DECORATORS_DOUBLEPLANTDECORATOR_H_

#include "Decorator.h"

struct DoublePlantDecoration {
  int weight;
  MinecraftBlock block;
};

class DoublePlantDecorator : public Decorator {
 public:
  void setDoublePlants(std::vector<DoublePlantDecoration> doublePlants);

  void decorate(SimpleChunkManager &world, Random &random, int chunkX, int chunkZ) override;
 private:
  MinecraftBlock getRandomDoublePlant(Random random);

  std::vector<DoublePlantDecoration> decorations;
};

#endif //EXT_NOISELIB_LIB_GENERATOR_DECORATORS_DOUBLEPLANTDECORATOR_H_
