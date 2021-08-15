#ifndef EXT_VANILLAGENERATOR_LIB_GENERATOR_OBJECTS_TREES_DEFAULTS_COCOATREE_H_
#define EXT_VANILLAGENERATOR_LIB_GENERATOR_OBJECTS_TREES_DEFAULTS_COCOATREE_H_

#include "JungleTree.h"

class CocoaTree : public JungleTree {
 public:
  bool Generate(ChunkManager &world, Random &random, int_fast32_t sourceX, int_fast32_t sourceY, int_fast32_t sourceZ) override;

 protected:
  void AddVinesOnTrunk(int_fast32_t trunkX, int_fast32_t trunkY, int_fast32_t trunkZ, ChunkManager &world, Random &random);
  void AddHangingVine(int_fast32_t x, int_fast32_t y, int_fast32_t z, int_fast32_t facing, ChunkManager &world);
  void AddVinesOnLeaves(int_fast32_t x, int_fast32_t y, int_fast32_t z, ChunkManager &world, Random &random);
  void AddCocoa(int_fast32_t sourceX, int_fast32_t sourceY, int_fast32_t sourceZ, ChunkManager &world, Random &random);

 private:
  constexpr static int_fast32_t SIZE_SMALL = 0;
  constexpr static int_fast32_t SIZE_MEDIUM = 1;
  constexpr static int_fast32_t SIZE_LARGE = 2;

  const static int_fast32_t COCOA_FACES[4];
  const static int_fast32_t COCOA_SIZE[3];
};

#endif //EXT_VANILLAGENERATOR_LIB_GENERATOR_OBJECTS_TREES_DEFAULTS_COCOATREE_H_
