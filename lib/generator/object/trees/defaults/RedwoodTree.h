#ifndef EXT_VANILLAGENERATOR_LIB_GENERATOR_OBJECTS_TREES_DEFAULTS_REDWOODTREE_H_
#define EXT_VANILLAGENERATOR_LIB_GENERATOR_OBJECTS_TREES_DEFAULTS_REDWOODTREE_H_

#include <lib/generator/object/trees/GenericTree.h>

class RedwoodTree : public GenericTree {
 public:
  void Initialize(Random &random, BlockTransaction &txn) override;

  bool Generate(ChunkManager &world, Random &random, int_fast32_t sourceX, int_fast32_t sourceY, int_fast32_t sourceZ) override;
 protected:
  bool CanPlace(int_fast32_t baseX, int_fast32_t baseY, int_fast32_t baseZ, ChunkManager &world) override;

  void SetMaxRadius(int_fast32_t maxRadius);
  void SetLeavesHeight(int_fast32_t leavesHeight);

  int_fast32_t maxRadius = 0;
  int_fast32_t leavesHeight = 0;
};

#endif //EXT_VANILLAGENERATOR_LIB_GENERATOR_OBJECTS_TREES_DEFAULTS_REDWOODTREE_H_
