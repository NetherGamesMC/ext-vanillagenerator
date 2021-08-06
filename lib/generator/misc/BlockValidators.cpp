#include "BlockValidators.h"

BlockTransaction::BlockTransaction(ChunkManager &manager) : world_(manager) {
  AddCallback(&VerifyLocationIntegrity);
}

void BlockTransaction::AddBlockAt(int_fast32_t x, int_fast32_t y, int_fast32_t z, MinecraftBlock block) {
  blocks_.insert({morton3d_encode(x, y, z), block});
}

MinecraftBlock BlockTransaction::FetchBlockAt(int_fast32_t x, int_fast32_t y, int_fast32_t z) {
  auto searchResult = blocks_.find(morton3d_encode(x, y, z));
  if (searchResult == blocks_.end()) {
    return world_.GetBlockAt(x, y, z);
  }

  return searchResult->second;
}

bool BlockTransaction::ApplyBlockChanges() {
  int_fast32_t x, z;
  int_fast32_t y;

  for (auto block : blocks_) {
    morton3d_decode(block.first, x, y, z);

    for (BlockValidators validator : validators_) {
      if (!validator(world_, x, y, z)) {
        return false;
      }
    }
  }

  for (auto block : blocks_) {
    morton3d_decode(block.first, x, y, z);

    world_.SetBlockAt(x, y, z, block.second);
  }

  return true;
}

void BlockTransaction::AddCallback(BlockValidators validator) {
  validators_.emplace_back(validator);
}

bool BlockTransaction::VerifyLocationIntegrity(ChunkManager &manager, int_fast32_t x, int_fast32_t y, int_fast32_t z) {
  return manager.IsInWorld(x, y, z);
}