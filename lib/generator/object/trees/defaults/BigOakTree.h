#ifndef EXT_VANILLAGENERATOR_LIB_GENERATOR_OBJECTS_TREES_DEFAULTS_BIGOAKTREE_H_
#define EXT_VANILLAGENERATOR_LIB_GENERATOR_OBJECTS_TREES_DEFAULTS_BIGOAKTREE_H_

#include <lib/generator/object/trees/GenericTree.h>
#include <lib/objects/math/Vector3.h>

struct LeafNode {
  int_fast32_t x = 0;
  int_fast32_t y = 0;
  int_fast32_t z = 0;
  int_fast32_t branchY = 0;
};

class BigOakTree : public GenericTree {
 public:
  void Initialize(Random &random, BlockTransaction &txn) override;

  bool Generate(ChunkManager &world, Random &random, int_fast32_t sourceX, int_fast32_t sourceY, int_fast32_t sourceZ) override;
 protected:
  void SetMaxLeafDistance(int_fast32_t distance);

  bool CanPlace(int_fast32_t baseX, int_fast32_t baseY, int_fast32_t baseZ, ChunkManager &world) override;
 private:
  int_fast32_t CountAvailableBlocks(Vector3 from, Vector3 to, ChunkManager &world);

  std::vector<LeafNode> GenerateLeafNodes(int_fast32_t blockX, int_fast32_t blockY, int_fast32_t blockZ, ChunkManager &world, Random &random);

  constexpr static float LEAF_DENSITY = 1.0F;

  int_fast32_t maxLeafDistance = 5;
  int_fast32_t trunkHeight = 0;
};

#endif //EXT_VANILLAGENERATOR_LIB_GENERATOR_OBJECTS_TREES_DEFAULTS_BIGOAKTREE_H_
