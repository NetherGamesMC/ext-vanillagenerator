#include "Block.h"
#include "lib/objects/math/Facing.h"

namespace blocks {

MCBlock::MCBlock(Block blockTypeId, int blockDataId, int blockTypeMaskId, int internalStateData) {
  blockType = blockTypeId;
  blockData = blockDataId;
  blockTypeMask = blockTypeMaskId;

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
  return blockType;
}

int MCBlock::GetStateId() const {
  return blockData ^ blockTypeMask;
}

int MCBlock::GetBlockMeta() const {
  return blockData;
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

thread_local std::map<int, MCBlock *> stateIdToBlocks;
thread_local std::map<int, int> typeIdXorMasks;

void MCBlock::RegisterBlock(int blockTypeId, int blockDataId, int internalStateData) {
  if (typeIdXorMasks.count(blockTypeId) == 0) {
    throw std::invalid_argument(std::to_string(blockTypeId) + " is not mapped in mask id map.");
  }

  int blockMaskId = typeIdXorMasks.at(blockTypeId);
  int blockHash = blockDataId ^ blockMaskId;

  if (stateIdToBlocks.count(blockHash) > 0) {
    throw std::invalid_argument(std::to_string(blockTypeId) + ":" + std::to_string(blockDataId) + " is already mapped");
  }

  if (blockTypeId > BlockIds::FIRST_UNUSED_BLOCK_ID) {
    BlockIds::FIRST_UNUSED_BLOCK_ID = blockTypeId;
  }

  stateIdToBlocks.insert({blockHash, new MCBlock(blockTypeId, blockDataId, blockMaskId, internalStateData)});
}

void MCBlock::RegisterMask(int blockTypeId, int blockMaskId) {
  if (typeIdXorMasks.count(blockTypeId) > 0) {
    throw std::invalid_argument(std::to_string(blockTypeId) + " is already mapped in mask id map.");
  }

  typeIdXorMasks.insert({blockTypeId, blockMaskId});
}

const MCBlock *MCBlock::GetBlockIdAndMeta(unsigned int blockStateId, unsigned int blockDataId) {
  auto mask = typeIdXorMasks.find(blockStateId);
  if (mask == typeIdXorMasks.end()) {
    throw std::logic_error(std::to_string(blockStateId) + " is not mapped in mask id map.");
  }

  int blockHash = blockDataId ^ mask->second;
  auto blockId = stateIdToBlocks.find(blockHash);
  if (blockId == stateIdToBlocks.end()) {
    printf("Block id is not registered for id : %d %d | %d \n", blockStateId, blockDataId, blockHash);
    throw std::logic_error("Block id is not registered for id: " + std::to_string(blockStateId) + " data: " + std::to_string(blockDataId));
  }

  return blockId->second;
}

const MCBlock *MCBlock::GetBlockFromStateId(unsigned int blockStateOrTypeId) {
  // This parameter is a block state id, directly reference the block object in our map.
  if (blockStateOrTypeId > BlockIds::FIRST_UNUSED_BLOCK_ID) {
    auto blockId = stateIdToBlocks.find(blockStateOrTypeId);
    if (blockId == stateIdToBlocks.end()) {
      printf("Block id is not registered for hash : %d \n", blockStateOrTypeId);
      throw std::logic_error("(GetBlockFromStateId:BlockState) Internal block id is not registered for hash: " + std::to_string(blockStateOrTypeId));
    }

    return blockId->second;
  }

  return GetBlockIdAndMeta(blockStateOrTypeId, 0);
}

bool MCBlock::operator==(const MCBlock *rhs) const {
  return GetTypeId() == rhs->GetTypeId();
}

bool MCBlock::operator!=(const MCBlock *rhs) const {
  return GetStateId() != rhs->GetStateId();
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