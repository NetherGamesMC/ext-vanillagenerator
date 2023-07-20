#ifndef EXT_VANILLAGENERATOR_LIB_GENERATOR_DECORATORS_UNDERWATERDECORATOR_H_
#define EXT_VANILLAGENERATOR_LIB_GENERATOR_DECORATORS_UNDERWATERDECORATOR_H_

#include "Decorator.h"

using namespace blocks;

class UnderwaterDecorator : public Decorator {
 public:
  UnderwaterDecorator(const MCBlock *mType);

  void SetOverridableBlocks(std::vector<const MCBlock *> fullBlockOverrides);

  void SetRadii(int_fast32_t iHorizontalRadius, int_fast32_t iVerticalRadius);

  void Decorate(ChunkManager &world, Random &random, int_fast32_t chunkX, int_fast32_t chunkZ) override;
 private:
  const MCBlock *type;

  int_fast32_t horizontalRadius = 0;
  int_fast32_t verticalRadius = 0;

  std::vector<const MCBlock *> overridable;
};

#endif //EXT_VANILLAGENERATOR_LIB_GENERATOR_DECORATORS_UNDERWATERDECORATOR_H_
