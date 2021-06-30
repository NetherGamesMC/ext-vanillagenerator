#ifndef EXT_NOISELIB_LIB_GENERATOR_DECORATORS_LAKEDECORATOR_H_
#define EXT_NOISELIB_LIB_GENERATOR_DECORATORS_LAKEDECORATOR_H_

#include "Decorator.h"
#include <lib/pocketmine/Logic.h>

class LakeDecorator : public Decorator {
 public:
  LakeDecorator(MinecraftBlock mcBlock, int populatorRarity, int offset = 0)
      : block(mcBlock), rarity(populatorRarity), baseOffset(offset) {}

  void Decorate(SimpleChunkManager &world, Random &random, int_fast64_t chunkX, int_fast64_t chunkZ) override;

 private:
  /**
   * Verifies if the coordinates has no water block intersecting to this decoration object.
   * If so, it will automatically cancels this decorator object.
   */
  static bool VerifyNoWaterBlocks(SimpleChunkManager &manager, int_fast64_t x, int_fast32_t y, int_fast64_t z) {
    return !IS_LIQUID(manager.getBlockAt(x, y, z).getId());
  }

  MinecraftBlock block;

  int rarity;
  int baseOffset = 0;
};

#endif //EXT_NOISELIB_LIB_GENERATOR_DECORATORS_LAKEDECORATOR_H_
