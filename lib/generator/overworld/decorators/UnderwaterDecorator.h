#ifndef EXT_NOISELIB_LIB_GENERATOR_DECORATORS_UNDERWATERDECORATOR_H_
#define EXT_NOISELIB_LIB_GENERATOR_DECORATORS_UNDERWATERDECORATOR_H_

#include <lib/objects/MinecraftBlock.h>
#include "Decorator.h"

class UnderwaterDecorator : public Decorator {
 public:
  UnderwaterDecorator(MinecraftBlock mType);

  void SetOverridableBlocks(std::vector<MinecraftBlock> fullBlockOverrides);

  void SetRadii(int iHorizontalRadius, int iVerticalRadius);

  void Decorate(ChunkManager &world, Random &random, int_fast32_t chunkX, int_fast32_t chunkZ) override;
 private:
  MinecraftBlock type;

  int horizontalRadius = 0;
  int verticalRadius = 0;

  std::vector<MinecraftBlock> overridable;
};

#endif //EXT_NOISELIB_LIB_GENERATOR_DECORATORS_UNDERWATERDECORATOR_H_
