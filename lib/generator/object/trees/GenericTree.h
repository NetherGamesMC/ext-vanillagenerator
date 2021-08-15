#ifndef EXT_VANILLAGENERATOR_LIB_GENERATOR_OBJECTS_GENERICTREE_H_
#define EXT_VANILLAGENERATOR_LIB_GENERATOR_OBJECTS_GENERICTREE_H_

#include <lib/objects/constants/BlockList.h>
#include <lib/generator/misc/BlockValidators.h>
#include <lib/generator/object/TerrainObjects.h>

#define MAGIC_NUMBER_OAK 0
#define MAGIC_NUMBER_SPRUCE 1
#define MAGIC_NUMBER_BIRCH 2
#define MAGIC_NUMBER_JUNGLE 3
#define MAGIC_NUMBER_ACACIA 4
#define MAGIC_NUMBER_DARK_OAK 5

class GenericTree : public TerrainObjects {
 public:
  virtual void Initialize(Random &random, BlockTransaction &txn);

  bool Generate(ChunkManager &world, Random &random, int_fast32_t sourceX, int_fast32_t sourceY, int_fast32_t sourceZ) override;

  void SetHeight(int_fast32_t blockHeight);
  void SetOverrides(std::vector<int> overridable);
  void SetType(int_fast32_t magicNumber);

 protected:
  virtual bool CanPlace(int_fast32_t baseX, int_fast32_t baseY, int_fast32_t baseZ, ChunkManager &world);
  virtual bool CanPlaceOn(MinecraftBlock soil);

  bool CannotGenerateAt(int_fast32_t baseX, int_fast32_t baseY, int_fast32_t baseZ, ChunkManager &world);

  void ReplaceIfAirOrLeaves(int_fast32_t x, int_fast32_t y, int_fast32_t z, MinecraftBlock newBlock, ChunkManager &world);

  [[nodiscard]] bool CanHeightFit(int_fast32_t base_height) const;

  int_fast32_t height = 0;
  BlockTransaction *transaction = nullptr;

  MinecraftBlock logType = OAK_WOOD;
  MinecraftBlock leavesTypes = OAK_LEAVES;

  std::vector<int> overrides = {0, 18, 2, 3, 17, 162, 6, 106};
};

struct TreeDecoration {
  int_fast32_t weight;
  GenericTree &treeObject;
};

#endif //EXT_VANILLAGENERATOR_LIB_GENERATOR_OBJECTS_GENERICTREE_H_
