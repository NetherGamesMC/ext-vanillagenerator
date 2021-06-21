#include "MinecraftBlock.h"

MinecraftBlock::MinecraftBlock(Block block) {
  blockId = block >> 4;
  meta = block & 0xf;
}

uint_fast32_t MinecraftBlock::getFullId() const {
  return (blockId << 4) | meta;
}

uint_fast16_t MinecraftBlock::getId() const {
  return blockId;
}

uint_fast8_t MinecraftBlock::getMeta() const {
  return meta;
}

MinecraftBlock MinecraftBlock::makeBlock(uint_fast8_t metadata, uint_fast8_t bitmask) const {
  assert((metadata & ~bitmask) == 0);

  uint_fast8_t blockMeta = getMeta() | metadata;
  return {getId(), blockMeta};
}

bool MinecraftBlock::isObjectNull() const {
  return getId() == 0 && getMeta() == 0;
}

bool MinecraftBlock::operator==(const MinecraftBlock &rhs) const {
  return rhs.getId() == getId() && rhs.getMeta() == getMeta();
}