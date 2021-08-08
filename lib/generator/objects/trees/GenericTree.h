#ifndef EXT_NOISELIB_LIB_GENERATOR_OBJECTS_GENERICTREE_H_
#define EXT_NOISELIB_LIB_GENERATOR_OBJECTS_GENERICTREE_H_

#include <lib/objects/constants/BlockList.h>
#include <lib/generator/misc/BlockValidators.h>
#include <lib/generator/objects/TerrainObjects.h>

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

  void SetHeight(int blockHeight);
  void SetOverrides(std::vector<int> overridable);
  void SetType(int magicNumber);

 protected:
  virtual bool CanPlaceOn(MinecraftBlock soil);

  bool CannotGenerateAt(int_fast32_t baseX, int_fast32_t baseY, int_fast32_t baseZ, ChunkManager &world);
  bool CanPlace(int_fast32_t baseX, int_fast32_t baseY, int_fast32_t baseZ, ChunkManager &world);

  void ReplaceIfAirOrLeaves(int_fast32_t x, int_fast32_t y, int_fast32_t z, MinecraftBlock newBlock, ChunkManager &world);

  bool CanHeightFit(int base_height) const;

  int_fast32_t height_ = 0;
  BlockTransaction *transaction_ = nullptr;

  MinecraftBlock logType_ = OAK_WOOD;
  MinecraftBlock leavesTypes_ = OAK_LEAVES;

  std::vector<int> overrides_ = {0, 18, 2, 3, 17, 162, 6, 106};
};

struct TreeDecoration {
  int weight;
  GenericTree &treeObject;
};

#endif //EXT_NOISELIB_LIB_GENERATOR_OBJECTS_GENERICTREE_H_
