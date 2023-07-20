#ifndef EXT_VANILLAGENERATOR_LIB_GENERATOR_DECORATORS_DOUBLEPLANTDECORATOR_H_
#define EXT_VANILLAGENERATOR_LIB_GENERATOR_DECORATORS_DOUBLEPLANTDECORATOR_H_

#include "Decorator.h"

using namespace blocks;

struct DoublePlantDecoration {
  int_fast32_t weight;
  const MCBlock *block;
};

class DoublePlantDecorator : public Decorator {
 public:
  void SetDoublePlants(std::vector<DoublePlantDecoration> doublePlants);

  void Decorate(ChunkManager &world, Random &random, int_fast32_t chunkX, int_fast32_t chunkZ) override;
 private:
  const MCBlock *GetRandomDoublePlant(Random random);

  std::vector<DoublePlantDecoration> decorations_;
};

#endif //EXT_VANILLAGENERATOR_LIB_GENERATOR_DECORATORS_DOUBLEPLANTDECORATOR_H_
