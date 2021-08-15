#ifndef EXT_VANILLAGENERATOR_LIB_OBJECTS_MINECRAFTBLOCK_H_
#define EXT_VANILLAGENERATOR_LIB_OBJECTS_MINECRAFTBLOCK_H_

#include <PhpPalettedBlockArrayObj.h>

class MinecraftBlock {
 public:
  MinecraftBlock(Block block);
  MinecraftBlock(uint_fast16_t block, uint_fast8_t blockMeta) : blockId(block), meta(blockMeta) {}

  /*
   * Returns the minecraft full block ids in unsigned int format.
   */
  auto GetFullId() const -> uint_fast32_t;

  /*
   * Returns the id for this item.
   */
  auto GetId() const -> uint_fast16_t;

  /*
   * Returns the meta for this item.
   */
  auto GetMeta() const -> uint_fast8_t;

  /*
   * Check either this block is null, which is usually an air.
   */
  auto IsObjectNull() const -> bool;

  /*
   * Comparison operator with another MinecraftBlock on the RHS.
   */
  bool operator==(const MinecraftBlock &rhs) const;

  /*
   * Comparison operator with another MinecraftBlock on the RHS.
   */
  bool operator!=(const MinecraftBlock &rhs) const;

  /*
   * Make a block based on the given metadata and bitmask.
   * The implementation is rawly based on PocketMine's Block::writeStateToMeta and
   * Block::getStateBitmask.
   */
  auto MakeBlock(uint_fast8_t metadata, uint_fast8_t bitmask) const -> MinecraftBlock;

  static uint_fast8_t writeLegacyHorizontalFacing(int facing);
  static uint_fast8_t writeVineBlockFacingMeta(int facing);
 private:
  uint_fast16_t blockId;
  uint_fast8_t meta;
};

#endif // EXT_VANILLAGENERATOR_LIB_OBJECTS_MINECRAFTBLOCK_H_
