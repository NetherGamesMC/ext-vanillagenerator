#ifndef EXT_VANILLAGENERATOR_LIB_GENERATOR_DECORATORS_LAKEDECORATOR_H_
#define EXT_VANILLAGENERATOR_LIB_GENERATOR_DECORATORS_LAKEDECORATOR_H_

#include "Decorator.h"
#include <lib/objects/constants/Logic.h>

class LakeDecorator : public Decorator {
 public:
  LakeDecorator(MinecraftBlock mcBlock, int_fast32_t populatorRarity, int_fast32_t offset = 0)
      : block(mcBlock), rarity(populatorRarity), baseOffset(offset) {}

  void Decorate(ChunkManager &world, Random &random, int_fast32_t chunkX, int_fast32_t chunkZ) override;

 private:
  /**
   * Verifies if the coordinates has no water block intersecting to this decoration object.
   * If so, it will automatically cancels this decorator object.
   */
  static bool VerifyNoWaterBlocks(ChunkManager &manager, int_fast32_t x, int_fast32_t y, int_fast32_t z) {
    return !IS_LIQUID(manager.GetBlockAt(x, y, z).GetId());
  }

  MinecraftBlock block;

  int_fast32_t rarity;
  int_fast32_t baseOffset = 0;
};

#endif //EXT_VANILLAGENERATOR_LIB_GENERATOR_DECORATORS_LAKEDECORATOR_H_
