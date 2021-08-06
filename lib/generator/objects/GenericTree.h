#ifndef EXT_NOISELIB_LIB_GENERATOR_OBJECTS_GENERICTREE_H_
#define EXT_NOISELIB_LIB_GENERATOR_OBJECTS_GENERICTREE_H_

#include <lib/objects/constants/BlockList.h>
#include <lib/generator/misc/BlockValidators.h>

#include "TerrainObjects.h"

class GenericTree : public TerrainObjects {
 public:
  GenericTree(Random &random, BlockTransaction &txn);

  bool Generate(ChunkManager &world, Random &random, int_fast32_t sourceX, int_fast32_t sourceY, int_fast32_t sourceZ) override;

 protected:

  void SetHeight(int blockHeight) { height_ = blockHeight; }

  void SetOverrides(std::vector<int> overridable) { overrides_ = std::move(overridable); }

  /**
   * Sets the block data values for this tree's blocks.
   *
   * @param TreeType $type
   */
  void SetType(int magicNumber) {
    log_type_ = MinecraftBlock(magicNumber >= 4 ? 162 : 17, magicNumber & 0x3);
    leaves_types_ = MinecraftBlock(magicNumber >= 4 ? 161 : 18, magicNumber & 0x3);
  }

 private:
  static bool CanPlaceOn(MinecraftBlock soil);

  bool CannotGenerateAt(int_fast32_t base_x, int_fast32_t base_y, int_fast32_t base_z, ChunkManager &world);

  void ReplaceIfAirOrLeaves(int_fast32_t x, int_fast32_t y, int_fast32_t z, MinecraftBlock newBlock, ChunkManager &world);

  bool CanPlace(int_fast32_t base_x, int_fast32_t base_y, int_fast32_t base_z, ChunkManager &world);

  [[nodiscard]] bool CanHeightFit(int base_height) const;

  int_fast32_t height_;
  BlockTransaction &transaction_;

  MinecraftBlock log_type_ = OAK_WOOD;
  MinecraftBlock leaves_types_ = OAK_LEAVES;

  std::vector<int> overrides_ = {0, 18, 2, 3, 17, 162, 6, 106};
};

typedef GenericTree (*TreeObject)(Random &, BlockTransaction &);

struct TreeDecoration {
  int weight;
  TreeObject callback;
};

static GenericTree defaultTree(Random &random, BlockTransaction &txn) { return {random, txn}; }

#endif //EXT_NOISELIB_LIB_GENERATOR_OBJECTS_GENERICTREE_H_
