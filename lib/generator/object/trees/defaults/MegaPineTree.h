#ifndef EXT_VANILLAGENERATOR_LIB_GENERATOR_OBJECTS_TREES_DEFAULTS_MEGAPINETREE_H_
#define EXT_VANILLAGENERATOR_LIB_GENERATOR_OBJECTS_TREES_DEFAULTS_MEGAPINETREE_H_

#include "MegaRedwoodTree.h"

class MegaPineTree : public MegaRedwoodTree {
 public:
  void Initialize(Random &random, BlockTransaction &txn) override;

  bool Generate(ChunkManager &world, Random &random, int_fast32_t sourceX, int_fast32_t sourceY, int_fast32_t sourceZ) override;
 protected:
  void GenerateDirtBelowTrunk(int_fast32_t blockX, int_fast32_t blockY, int_fast32_t blockZ) override;

 private:
  static void GeneratePodzol(int_fast32_t sourceX, int_fast32_t sourceY, int_fast32_t sourceZ, ChunkManager &world, Random &random);
  static void GeneratePodzolPatch(int_fast32_t sourceX, int_fast32_t sourceY, int_fast32_t sourceZ, ChunkManager &world);
};

#endif //EXT_VANILLAGENERATOR_LIB_GENERATOR_OBJECTS_TREES_DEFAULTS_MEGAPINETREE_H_
