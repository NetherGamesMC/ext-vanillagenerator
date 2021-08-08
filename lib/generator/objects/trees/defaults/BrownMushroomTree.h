#ifndef EXT_NOISELIB_LIB_GENERATOR_OBJECTS_TREES_DEFAULTS_BROWNMUSHROOMTREE_H_
#define EXT_NOISELIB_LIB_GENERATOR_OBJECTS_TREES_DEFAULTS_BROWNMUSHROOMTREE_H_

#include <lib/generator/objects/trees/GenericTree.h>

class BrownMushroomTree : public GenericTree {
 public:
  void Initialize(Random &random, BlockTransaction &txn) override;

  bool CanPlaceOn(MinecraftBlock soil) override;


 protected:
  MinecraftBlock type = BROWN_MUSHROOM_BLOCK;
};

#endif //EXT_NOISELIB_LIB_GENERATOR_OBJECTS_TREES_DEFAULTS_BROWNMUSHROOMTREE_H_
