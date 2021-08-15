#ifndef EXT_VANILLAGENERATOR_LIB_GENERATOR_OBJECTS_TREES_DEFAULTS_MEGAREDWOODTREE_H_
#define EXT_VANILLAGENERATOR_LIB_GENERATOR_OBJECTS_TREES_DEFAULTS_MEGAREDWOODTREE_H_

#include "MegaJungleTree.h"

class MegaRedwoodTree : public MegaJungleTree {
 public:
  void Initialize(Random &random, BlockTransaction &txn) override;

  bool Generate(ChunkManager &world, Random &random, int_fast32_t sourceX, int_fast32_t sourceY, int_fast32_t sourceZ) override;
 protected:
  void SetLeavesHeight(int_fast32_t iLeavesHeight);

  void GenerateDirtBelowTrunk(int_fast32_t blockX, int_fast32_t blockY, int_fast32_t blockZ) override;

  int_fast32_t leavesHeight = 0;
};

#endif //EXT_VANILLAGENERATOR_LIB_GENERATOR_OBJECTS_TREES_DEFAULTS_MEGAREDWOODTREE_H_
