#include <lib/MortonHelper.h>
#include <lib/chunk/ChunkManager.h>

Chunk *ChunkManager::GetChunk(int_fast32_t chunkX, int_fast32_t chunkZ) {
  uint_fast64_t location = morton2d_encode(chunkX, chunkZ);

  auto searchResult = chunks.find(location);
  if (searchResult == chunks.end()) {
    return nullptr;
  }

  return searchResult->second;
}

void ChunkManager::SetChunk(int_fast32_t chunkX, int_fast32_t chunkZ, Chunk *chunk) {
  chunks.insert({morton2d_encode(chunkX, chunkZ), chunk});
}

const blocks::MCBlock *ChunkManager::GetBlockAt(int_fast32_t x, int_fast32_t y, int_fast32_t z) {
  using namespace blocks;

  Chunk *chunk;
  if (IsInWorld(x, y, z) && (chunk = GetChunk(x >> 4, z >> 4)) != nullptr) {
    return MCBlock::GetBlockFromStateId(chunk->GetFullBlock(x & 0xf, y,z & 0xf));
  }

  return MCBlock::GetBlockFromStateId(BlockIds::AIR);
}

void ChunkManager::SetBlockAt(int_fast32_t x, int_fast32_t y, int_fast32_t z, const blocks::MCBlock *block) {
  using namespace std;
  using namespace blocks;

  Chunk *chunk;
  if ((chunk = GetChunk(x >> 4, z >> 4)) != nullptr) {
    chunk->SetFullBlock(x & 0xf, y, z & 0xf, block->GetStateId());
  } else {
    throw std::invalid_argument("Cannot set block at coordinates x=" + to_string(x) + ", y=" + to_string(y) + ", z=" + to_string(z) + ", terrain is not loaded or out of bounds");
  }
}

bool ChunkManager::IsInWorld(int_fast32_t x, int_fast32_t y, int_fast32_t z) const {
  return x <= INT32_MAX && x >= INT32_MIN && y < maxY && y >= minY && z <= INT32_MAX && z >= INT32_MIN;
}

int_fast32_t ChunkManager::GetMinY() const {
  return minY;
}

int_fast32_t ChunkManager::GetMaxY() const {
  return maxY;
}

std::map<uint_fast64_t, Chunk *> ChunkManager::GetChunks() const {
  return chunks;
}

const blocks::MCBlock *ChunkManager::GetHighestBlockAt(int_fast32_t x, int_fast32_t z) {
  using namespace blocks;

  Chunk *chunk;
  if (IsInWorld(x, 0, z) && (chunk = GetChunk(x >> 4, z >> 4)) != nullptr) {
    return MCBlock::GetBlockFromStateId(chunk->GetFullBlock(x & 0xf, chunk->GetHighestBlockAt(x & 0xf, z & 0xf), z & 0xf));
  }

  return MCBlock::GetBlockFromStateId(BlockIds::AIR);
}

int_fast32_t ChunkManager::GetHighestElevationAt(int_fast32_t x, int_fast32_t z) {
  Chunk *chunk;
  if (IsInWorld(x, 0, z) && (chunk = GetChunk(x >> 4, z >> 4)) != nullptr) {
    return chunk->GetHighestBlockAt(x & 0xf, z & 0xf);
  }

  return Chunk::Y_MIN - 1;
}
