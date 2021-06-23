#ifndef EXT_NOISELIB_LIB_GENERATOR_MISC_BLOCKVALIDATORS_H_
#define EXT_NOISELIB_LIB_GENERATOR_MISC_BLOCKVALIDATORS_H_

#include <lib/chunk/SimpleChunkManager.h>
#include <lib/MortonHelper.h>

typedef bool (*BlockValidators)(SimpleChunkManager &, int, int, int);

class BlockTransaction {
 public:
  explicit BlockTransaction(SimpleChunkManager &manager) : world(manager) {
    addCallback(&verifyLocationIntegrity);
  }

  void addBlockAt(int_fast64_t x, int_fast16_t y, int_fast64_t z, MinecraftBlock *block) {
    blocks[morton3d_encode(x, y, z)] = block;
  }

  /**
   * @see BlockTransaction::fetchBlock()
   */
  MinecraftBlock fetchBlockAt(int_fast64_t x, int_fast16_t y, int_fast64_t z) {
    auto searchResult = blocks.find(morton3d_encode(x, y, z));
    if (searchResult == blocks.end()) {
      return world.getBlockAt(x, y, z);
    }

    return *searchResult->second;
  }

  /**
   * Validates and attempts to apply the transaction to the given world. If any part of the transaction fails to
   * validate, no changes will be made to the world.
   *
   * @return bool if the application was successful
   */
  bool applyBlockChanges() {
    int64_t x, y, z;

    for (auto block : blocks) {
      morton3d_decode(static_cast<int64_t>(block.first), x, y, z);

      for (BlockValidators validator : validators) {
        if (!validator(world, static_cast<int>(x), static_cast<int>(y), static_cast<int>(z))) {
          return false;
        }
      }
    }

    for (auto block : blocks) {
      morton3d_decode(static_cast<int64_t>(block.first), x, y, z);

      world.setBlockAt(static_cast<int_fast64_t>(x), static_cast<int_fast16_t>(y), static_cast<int_fast64_t>(z), *block.second);
    }

    return true;
  }

  /**
   * Add a validation predicate which will be used to validate every block.
   * The callable signature should be the same as the below function.
   */
  void addCallback(BlockValidators validator) {
    validators.emplace_back(validator);
  }

  void destroy() {
    for (auto block : blocks) {
      delete block.second;
    }

    blocks.clear();
  }

 private:
  /**
   * This is a callback to verify the locations of the given coordinates. It does a
   * simple checks if the location is within the world radius.
   */
  static bool verifyLocationIntegrity(SimpleChunkManager &manager, int x, int y, int z) {
    return manager.isInWorld(x, y, z);
  }

  SimpleChunkManager world;

  std::vector<BlockValidators> validators;
  std::map<uint_fast64_t, MinecraftBlock *> blocks;
};

#endif // EXT_NOISELIB_LIB_GENERATOR_MISC_BLOCKVALIDATORS_H_
