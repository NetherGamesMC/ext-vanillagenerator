#ifndef EXT_VANILLAGENERATOR_LIB_GENERATOR_MISC_BLOCKVALIDATORS_H_
#define EXT_VANILLAGENERATOR_LIB_GENERATOR_MISC_BLOCKVALIDATORS_H_

#include <lib/chunk/ChunkManager.h>
#include <lib/MortonHelper.h>
#include <lib/objects/constants/Logic.h>

class BlockTransaction {
 public:
  typedef bool (*BlockValidators)(ChunkManager &, int_fast32_t, int_fast32_t, int_fast32_t);

  explicit BlockTransaction(ChunkManager &manager);

  void AddBlockAt(int_fast32_t x, int_fast32_t y, int_fast32_t z, MinecraftBlock block);

  /**
   * @see BlockTransaction::fetchBlock()
   */
  MinecraftBlock FetchBlockAt(int_fast32_t x, int_fast32_t y, int_fast32_t z);

  /**
   * Validates and attempts to apply the transaction to the given world. If any part of the transaction fails to
   * Validate, no changes will be made to the world.
   *
   * @return bool if the application was successful
   */
  bool ApplyBlockChanges();

  /**
   * Add a validation predicate which will be used to Validate every block.
   * The callable signature should be the same as the below function.
   */
  void AddCallback(BlockValidators validator);

 private:
  /**
   * This is a callback to verify the locations of the given coordinates. It does a
   * simple checks if the location is within the world radius.
   */
  static bool VerifyLocationIntegrity(ChunkManager &manager, int_fast32_t x, int_fast32_t y, int_fast32_t z);

  ChunkManager &world_;

  std::vector<BlockValidators> validators_;
  std::map<uint_fast64_t, const MinecraftBlock> blocks_;
};

#endif // EXT_VANILLAGENERATOR_LIB_GENERATOR_MISC_BLOCKVALIDATORS_H_
