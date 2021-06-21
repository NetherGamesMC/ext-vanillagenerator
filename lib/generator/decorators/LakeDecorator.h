#ifndef EXT_NOISELIB_LIB_GENERATOR_DECORATORS_LAKEDECORATOR_H_
#define EXT_NOISELIB_LIB_GENERATOR_DECORATORS_LAKEDECORATOR_H_

#include "Decorator.h"

class LakeDecorator : public Decorator {
 public:
  LakeDecorator(MinecraftBlock mcBlock, int populatorRarity, int offset = 0)
      : block(mcBlock), rarity(populatorRarity), baseOffset(offset) {
    // NOOP
  }

  void decorate(SimpleChunkManager &world, Random &random, int chunkX, int chunkZ) override;

 private:
  MinecraftBlock block;

  int rarity;
  int baseOffset = 0;
};

#endif //EXT_NOISELIB_LIB_GENERATOR_DECORATORS_LAKEDECORATOR_H_
