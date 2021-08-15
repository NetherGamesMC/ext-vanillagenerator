#ifndef EXT_VANILLAGENERATOR_LIB_GENERATOR_OBJECTS_TREES_DEFAULTS_BROWNMUSHROOMTREE_H_
#define EXT_VANILLAGENERATOR_LIB_GENERATOR_OBJECTS_TREES_DEFAULTS_BROWNMUSHROOMTREE_H_

#include <lib/generator/object/trees/GenericTree.h>

class BrownMushroomTree : public GenericTree {
 public:
  void Initialize(Random &random, BlockTransaction &txn) override;

  bool Generate(ChunkManager &world, Random &random, int_fast32_t sourceX, int_fast32_t sourceY, int_fast32_t sourceZ) override;
 protected:
  bool CanPlaceOn(MinecraftBlock soil) override;
  bool CanPlace(int_fast32_t baseX, int_fast32_t baseY, int_fast32_t baseZ, ChunkManager &world) override;

  MinecraftBlock type = BROWN_MUSHROOM_BLOCK;
};

#endif //EXT_VANILLAGENERATOR_LIB_GENERATOR_OBJECTS_TREES_DEFAULTS_BROWNMUSHROOMTREE_H_
