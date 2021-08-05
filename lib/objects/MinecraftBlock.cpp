#include "MinecraftBlock.h"

MinecraftBlock::MinecraftBlock(Block block) {
  blockId = block >> 4;
  meta = block & 0xf;
}

uint_fast32_t MinecraftBlock::GetFullId() const {
  return (blockId << 4) | meta;
}

uint_fast16_t MinecraftBlock::GetId() const {
  return blockId;
}

uint_fast8_t MinecraftBlock::GetMeta() const {
  return meta;
}

MinecraftBlock MinecraftBlock::MakeBlock(uint_fast8_t metadata, uint_fast8_t bitmask) const {
  assert((metadata & ~bitmask) == 0);

  uint_fast8_t blockMeta = GetMeta() | metadata;
  return {GetId(), blockMeta};
}

bool MinecraftBlock::IsObjectNull() const {
  return GetId() == 0 && GetMeta() == 0;
}

bool MinecraftBlock::operator==(const MinecraftBlock &rhs) const {
  return rhs.GetId() == GetId() && rhs.GetMeta() == GetMeta();
}