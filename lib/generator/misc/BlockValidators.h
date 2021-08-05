#ifndef EXT_NOISELIB_LIB_GENERATOR_MISC_BLOCKVALIDATORS_H_
#define EXT_NOISELIB_LIB_GENERATOR_MISC_BLOCKVALIDATORS_H_

#include <lib/chunk/ChunkManager.h>
#include <lib/MortonHelper.h>
#include <lib/objects/constants/Logic.h>

typedef bool (*BlockValidators)(ChunkManager &, int_fast64_t, int_fast32_t, int_fast64_t);

class BlockTransaction {
 public:
  explicit BlockTransaction(ChunkManager &manager) : world_(manager) {
    AddCallback(&VerifyLocationIntegrity);
  }

  void AddBlockAt(int_fast64_t x, int_fast32_t y, int_fast64_t z, const MinecraftBlock block) {
    blocks_.insert({morton3d_encode(x, y, z), block});
  }

  /**
   * @see BlockTransaction::fetchBlock()
   */
  MinecraftBlock FetchBlockAt(int_fast64_t x, int_fast32_t y, int_fast64_t z) {
    auto searchResult = blocks_.find(morton3d_encode(x, y, z));
    if (searchResult == blocks_.end()) {
      return world_.getBlockAt(x, y, z);
    }

    return searchResult->second;
  }

  /**
   * Validates and attempts to apply the transaction to the given world. If any part of the transaction fails to
   * validate, no changes will be made to the world.
   *
   * @return bool if the application was successful
   */
  bool ApplyBlockChanges() {
    int_fast64_t x, z;
    int_fast32_t y;

    for (auto block : blocks_) {
      morton3d_decode(static_cast<int64_t>(block.first), x, y, z);

      for (BlockValidators validator : validators_) {
        if (!validator(world_, x, y, z)) {
          return false;
        }
      }
    }

    for (auto block : blocks_) {
      morton3d_decode(static_cast<int64_t>(block.first), x, y, z);

      world_.setBlockAt(x, y, z, block.second);
    }

    return true;
  }

  /**
   * Add a validation predicate which will be used to validate every block.
   * The callable signature should be the same as the below function.
   */
  void AddCallback(BlockValidators validator) {
    validators_.emplace_back(validator);
  }

 private:
  /**
   * This is a callback to verify the locations of the given coordinates. It does a
   * simple checks if the location is within the world radius.
   */
  static bool VerifyLocationIntegrity(ChunkManager &manager, int_fast64_t x, int_fast32_t y, int_fast64_t z) {
    return manager.isInWorld(x, y, z);
  }

  ChunkManager world_;

  std::vector<BlockValidators> validators_;
  std::map<uint_fast64_t, const MinecraftBlock> blocks_;
};

#endif // EXT_NOISELIB_LIB_GENERATOR_MISC_BLOCKVALIDATORS_H_
