#ifndef EXT_VANILLAGENERATOR_LIB_GENERATOR_DECORATORS_UNDERWATERDECORATOR_H_
#define EXT_VANILLAGENERATOR_LIB_GENERATOR_DECORATORS_UNDERWATERDECORATOR_H_

#include <lib/objects/MinecraftBlock.h>
#include "Decorator.h"

class UnderwaterDecorator : public Decorator {
 public:
  UnderwaterDecorator(MinecraftBlock mType);

  void SetOverridableBlocks(std::vector<MinecraftBlock> fullBlockOverrides);

  void SetRadii(int_fast32_t iHorizontalRadius, int_fast32_t iVerticalRadius);

  void Decorate(ChunkManager &world, Random &random, int_fast32_t chunkX, int_fast32_t chunkZ) override;
 private:
  MinecraftBlock type;

  int_fast32_t horizontalRadius = 0;
  int_fast32_t verticalRadius = 0;

  std::vector<MinecraftBlock> overridable;
};

#endif //EXT_VANILLAGENERATOR_LIB_GENERATOR_DECORATORS_UNDERWATERDECORATOR_H_
