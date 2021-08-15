#ifndef EXT_VANILLAGENERATOR_LIB_GENERATOR_OBJECTS_TREES_DEFAULTS_ACACIATREE_H_
#define EXT_VANILLAGENERATOR_LIB_GENERATOR_OBJECTS_TREES_DEFAULTS_ACACIATREE_H_

#include <lib/generator/object/trees/GenericTree.h>

class AcaciaTree : public GenericTree {
 public:
  void Initialize(Random &random, BlockTransaction &txn) override;

  bool CanPlaceOn(MinecraftBlock soil) override;

  bool Generate(ChunkManager &world, Random &random, int_fast32_t sourceX, int_fast32_t sourceY, int_fast32_t sourceZ) override;
 private:
  void SetLeaves(int_fast32_t x, int_fast32_t y, int_fast32_t z, ChunkManager &world);
};

#endif //EXT_VANILLAGENERATOR_LIB_GENERATOR_OBJECTS_TREES_DEFAULTS_ACACIATREE_H_
