#include "Block.h"
#include "lib/objects/math/Facing.h"

namespace blocks {

MCBlock::MCBlock(Block fullBlockStateId, int blockMetaId, int internalStateData) {
  blockStateId = fullBlockStateId;
  blockMeta = blockMetaId;

  // 1st bit = solid state
  // 2nd bit = transparent state
  // 3rd bit = flowable state
  // 4th bit = liquid state
  // 5th bit = light level (require 4 bits, 0-15)

  // 0000.0.0.0.0 (Total 8 bits)

  isSolid = ((internalStateData >> 0) & 1) == 1;
  isTransparent = ((internalStateData >> 1) & 1) == 1;
  isFlowable = ((internalStateData >> 2) & 1) == 1;
  isLiquid = ((internalStateData >> 3) & 1) == 1;
  blockLightLevel = (internalStateData >> 4) & ~(~0 << 4);
}

int MCBlock::GetTypeId() const {
  return blockStateId;
}

int MCBlock::GetStateId() const {
  return (GetTypeId() << INTERNAL_STATE_DATA_BITS) | (GetBlockMeta() ^ (GetTypeId() & INTERNAL_STATE_DATA_MASK));
}

int MCBlock::GetBlockMeta() const {
  return blockMeta;
}

bool MCBlock::IsSolid() const {
  return isSolid;
}

bool MCBlock::IsTransparent() const {
  return isTransparent;
}

bool MCBlock::IsFlowable() const {
  return isFlowable;
}

bool MCBlock::IsLiquid() const {
  return isLiquid;
}

int MCBlock::GetLightLevel() const {
  return blockLightLevel;
}

void MCBlock::RegisterBlock(int id, int meta, int internalStateData) {
  unsigned int index = (id << INTERNAL_STATE_DATA_BITS) | (meta ^ (id & INTERNAL_STATE_DATA_MASK));

  if (stateIdToBlocks.count(index) > 0) {
    throw std::invalid_argument(std::to_string(id) + ":" + std::to_string(meta) + " is already mapped");
  }

  if (id > BlockIds::FIRST_UNUSED_BLOCK_ID) {
    BlockIds::FIRST_UNUSED_BLOCK_ID = id;
  }

  stateIdToBlocks.insert({index, new MCBlock(id, meta, internalStateData)});
}

MCBlock const *MCBlock::GetBlockIdAndMeta(unsigned int blockStateId, unsigned int meta) {
  int blockHash = (blockStateId << INTERNAL_STATE_DATA_BITS) | (meta ^ (blockStateId & INTERNAL_STATE_DATA_MASK));
  if (stateIdToBlocks.count(blockHash) > 0) {
    return stateIdToBlocks.at(blockHash);
  }

  printf("Block id is not registered for id : %d %d | %d \n", blockStateId, meta, blockHash);
  throw std::logic_error("Block id is not registered for id: " + std::to_string(blockStateId) + " meta: " + std::to_string(meta));
}

const MCBlock *MCBlock::GetBlockFromStateId(unsigned int internalBlockStateId) {
  if (internalBlockStateId > BlockIds::FIRST_UNUSED_BLOCK_ID) {
    int typeId = internalBlockStateId >> INTERNAL_STATE_DATA_BITS;
    int stateData = (internalBlockStateId ^ typeId) & INTERNAL_STATE_DATA_MASK;

    return GetBlockIdAndMeta(typeId, stateData);
  }

  unsigned int index = (internalBlockStateId << INTERNAL_STATE_DATA_BITS) | (0 ^ (internalBlockStateId & INTERNAL_STATE_DATA_MASK));

  if (stateIdToBlocks.count(index) > 0) {
    return stateIdToBlocks.at(index);
  }

  printf("Block id is not registered for id : %d | %d\n", index, internalBlockStateId);

  throw std::logic_error("Internal block id is not registered for id: " + std::to_string(index));
}

bool MCBlock::operator==(const MCBlock *rhs) const {
  return GetTypeId() == rhs->GetTypeId();
}

bool MCBlock::operator!=(const MCBlock *rhs) const {
  return GetStateId() != rhs->GetStateId();
}

int MCBlock::WriteLegacyHorizontalFacing(int facing) {
  switch (facing) {
    case Facing::SOUTH: return 0;
    case Facing::WEST: return 1;
    case Facing::NORTH: return 2;
    case Facing::EAST: return 3;
    default: return 0;
  }
}

int MCBlock::WriteVineBlockFacingMeta(int facing) {
  switch (facing) {
    case Facing::NORTH: return 4;
    case Facing::SOUTH: return 1;
    case Facing::WEST: return 2;
    case Facing::EAST: return 8;
    default: return 0;
  }
}

}