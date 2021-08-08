#ifndef EXT_NOISELIB_LIB_GENERATOR_OBJECTS_TREES_DEFAULTS_BIGOAKTREE_H_
#define EXT_NOISELIB_LIB_GENERATOR_OBJECTS_TREES_DEFAULTS_BIGOAKTREE_H_

#include <lib/generator/objects/trees/GenericTree.h>
#include <lib/objects/math/Vector3.h>

struct LeafNode {
  int x = 0;
  int y = 0;
  int z = 0;
  int branchY = 0;
};

class BigOakTree : public GenericTree {
 public:
  void Initialize(Random &random, BlockTransaction &txn) override;

 protected:
  void SetMaxLeafDistance(int distance);

  bool CanPlaceOn(MinecraftBlock soil) override;
 private:
  int CountAvailableBlocks(Vector3 from, Vector3 to, ChunkManager &world);

  constexpr static float LEAF_DENSITY = 1.0F;

  int maxLeafDistance = 5;
  int trunkHeight = 0;
};

#endif //EXT_NOISELIB_LIB_GENERATOR_OBJECTS_TREES_DEFAULTS_BIGOAKTREE_H_
