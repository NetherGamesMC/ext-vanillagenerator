#ifndef EXT_NOISELIB_LIB_GENERATOR_OBJECTS_GENERICTREE_H_
#define EXT_NOISELIB_LIB_GENERATOR_OBJECTS_GENERICTREE_H_

#include <lib/pocketmine/BlockList.h>
#include <lib/generator/misc/BlockValidators.h>

#include "TerrainObjects.h"

class GenericTree : public TerrainObjects {
 public:
  GenericTree(Random &random, BlockTransaction &txn) : transaction(txn) {
    height = static_cast<int>(random.nextBoundedInt(3) + 4);
  }

  bool generate(SimpleChunkManager world, Random &random, int source_x, int source_y, int source_z) override;

 protected:

  void setHeight(int blockHeight) {
    height = blockHeight;
  }

  void setOverrides(std::vector<int> overridable) {
    overrides = std::move(overridable);
  }

  /**
   * Sets the block data values for this tree's blocks.
   *
   * @param TreeType $type
   */
  void setType(int magicNumber) {
    logType = MinecraftBlock(magicNumber >= 4 ? 162 : 17, magicNumber & 0x3);
    leavesTypes = MinecraftBlock(magicNumber >= 4 ? 161 : 18, magicNumber & 0x3);
  }

 private:
  static bool canPlaceOn(MinecraftBlock soil);

  bool canHeightFit(int base_height) const;

  bool cannotGenerateAt(int base_x, int base_y, int base_z, SimpleChunkManager world);

  void replaceIfAirOrLeaves(int x, int y, int z, MinecraftBlock newBlock, SimpleChunkManager world);

  bool canPlace(int base_x, int base_y, int base_z, SimpleChunkManager world);

  int height;
  BlockTransaction &transaction;

  MinecraftBlock logType = OAK_WOOD;
  MinecraftBlock leavesTypes = OAK_LEAVES;

  std::vector<int> overrides = {0, 18, 2, 3, 17, 162, 6, 106};
};

typedef GenericTree (*TreeObject)(Random &, BlockTransaction &);

struct TreeDecoration {
  int weight;
  TreeObject callback;
};

static GenericTree defaultTree(Random &random, BlockTransaction &txn) {
  return GenericTree(random, txn);
}

#endif //EXT_NOISELIB_LIB_GENERATOR_OBJECTS_GENERICTREE_H_
