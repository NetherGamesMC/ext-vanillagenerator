#ifndef EXT_VANILLAGENERATOR_LIB_GENERATOR_OBJECTS_TREES_DEFAULTS_MEGAJUNGLETREE_H_
#define EXT_VANILLAGENERATOR_LIB_GENERATOR_OBJECTS_TREES_DEFAULTS_MEGAJUNGLETREE_H_

#include <lib/generator/object/trees/GenericTree.h>

class MegaJungleTree : public GenericTree {
 public:
  void Initialize(Random &random, BlockTransaction &txn) override;

  bool Generate(ChunkManager &world, Random &random, int_fast32_t sourceX, int_fast32_t sourceY, int_fast32_t sourceZ) override;
 protected:
  bool CanPlace(int_fast32_t baseX, int_fast32_t baseY, int_fast32_t baseZ, ChunkManager &world) override;
  bool CanPlaceOn(MinecraftBlock soil) override;

  void GenerateLeaves(int_fast32_t sourceX, int_fast32_t sourceY, int_fast32_t sourceZ, int_fast32_t radius, bool odd, ChunkManager &world);
  void GenerateTrunk(ChunkManager &world, int_fast32_t blockX, int_fast32_t blockY, int_fast32_t blockZ);

  virtual void GenerateDirtBelowTrunk(int_fast32_t blockX, int_fast32_t blockY, int_fast32_t blockZ);

 private:
  void AddVinesOnTrunk(ChunkManager &world, int_fast32_t blockX, int_fast32_t blockY, int_fast32_t blockZ, Random &random);
  void MaybePlaceVine(ChunkManager &world, int_fast32_t absoluteX, int_fast32_t absoluteY, int_fast32_t absoluteZ, int_fast32_t direction, Random &random);
};

#endif //EXT_VANILLAGENERATOR_LIB_GENERATOR_OBJECTS_TREES_DEFAULTS_MEGAJUNGLETREE_H_
