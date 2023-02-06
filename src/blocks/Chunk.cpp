#include <iostream>
#include <blocks/Chunk.h>
#include <blocks/Blocks.h>
#include <math/Math.h>

using namespace math;
using namespace block;
using namespace chunk;

uint8_t BiomeArray::Get(const uint8_t x, const uint8_t z) const {
  uint_fast8_t offset;
  Index(x, z, offset);

  return (*biomes_)[offset];
}

void BiomeArray::Set(const uint8_t x, const uint8_t z, const uint8_t value) {
  uint_fast8_t offset;
  Index(x, z, offset);

  (*biomes_)[offset] = value;
}

void BiomeArray::Index(const uint8_t x, const uint8_t z, uint_fast8_t &offset) {
  offset = (z << 4) | x;
}

void BiomeArray::SetBiomeData(BiomeList *biome_list) {
  biomes_ = biome_list;
}

BiomeList *BiomeArray::GetRawData() const {
  return biomes_;
}

ChunkManager::ChunkManager(const int_fast32_t min_height, const int_fast32_t max_height)
    : min_y_(min_height), max_y_(max_height) {}

Chunk *ChunkManager::GetChunk(int_fast64_t chunk_x, int_fast64_t chunk_z) {
  uint_fast64_t location = Math::morton2d_encode(chunk_x, chunk_z);

  auto searchResult = chunks_.find(location);
  if (searchResult == chunks_.end()) {
    return nullptr;
  }

  return searchResult->second;
}

void ChunkManager::SetChunk(int_fast64_t chunk_x, int_fast64_t chunk_z, Chunk *chunk) {
  chunks_.insert({Math::morton2d_encode(chunk_x, chunk_z), chunk});
}

const Block *ChunkManager::GetBlockAt(int_fast32_t x, int_fast32_t y, int_fast32_t z) {
  Chunk *chunk;
  if (IsInWorld(x, y, z) && (chunk = GetChunk(x >> 4, z >> 4)) != nullptr) {
    return chunk->GetFullBlock(static_cast<int_fast8_t>(x & 0xf),
                               static_cast<int_fast16_t>(y),
                               static_cast<int_fast8_t>(z & 0xf));
  }

  return Block::GetBlockIdAndMeta(0, 0);
}

void ChunkManager::SetBlockAt(int_fast32_t x, int_fast32_t y, int_fast32_t z, const Block *block) {
  Chunk *chunk;

  if ((chunk = GetChunk(x >> 4, z >> 4)) != nullptr) {
    chunk->SetFullBlock(static_cast<int_fast8_t>(x & 0xf),
                        static_cast<int_fast16_t>(y),
                        static_cast<int_fast8_t>(z & 0xf),
                        block);
  } else {
    throw std::invalid_argument(
        "Cannot set block at coordinates x=" + std::to_string(x) + ", y=" + std::to_string(y) + ", z="
            + std::to_string(z) + ", terrain is not loaded or out of bounds");
  }
}

bool ChunkManager::IsInWorld(int_fast32_t x, int_fast32_t y, int_fast32_t z) const {
  return x <= INT32_MAX && x >= INT32_MIN && y < max_y_ && y >= min_y_ && z <= INT32_MAX && z >= INT32_MIN;
}

int_fast32_t ChunkManager::GetMinY() const {
  return min_y_;
}

int_fast32_t ChunkManager::GetMaxY() const {
  return max_y_;
}

ChunkManager::~ChunkManager() {
  for (auto data : chunks_) {
    delete data.second;
  }

  chunks_.clear();
}

std::map<uint_fast64_t, Chunk *> ChunkManager::GetChunks() const {
  return chunks_;
}

const Block *ChunkManager::GetHighestBlockAt(int_fast32_t x, int_fast32_t z) {
  Chunk *chunk;
  if (IsInWorld(x, 0, z) && (chunk = GetChunk(x >> 4, z >> 4)) != nullptr) {
    return chunk->GetFullBlock(static_cast<int_fast8_t>(x & 0xf),
                               chunk->GetHighestBlockAt(x & 0xf, z & 0xf),
                               static_cast<int_fast8_t>(z & 0xf));
  }

  return Block::GetBlockIdAndMeta(0, 0);
}

int_fast32_t ChunkManager::GetHighestElevationAt(int_fast32_t x, int_fast32_t z) {
  Chunk *chunk;
  if (IsInWorld(x, 0, z) && (chunk = GetChunk(x >> 4, z >> 4)) != nullptr) {
    return chunk->GetHighestBlockAt(x & 0xf, z & 0xf);
  }

  return 0;
}

Chunk::Chunk() : block_layer_(), biome_array_() {
  chunk_x_ = 0;
  chunk_z_ = 0;
}

SubChunk *Chunk::GetSubChunk(uint_fast8_t y) {
  if (y >= block_layer_.size()) {
    return nullptr;
  }

  return block_layer_[y];
}

void Chunk::SetFullBlock(int_fast8_t x, int_fast16_t y, int_fast8_t z, const Block *block) {
  SubChunk *subChunk;
  if ((subChunk = GetSubChunk(y >> 4)) == nullptr) {
    throw std::invalid_argument("Subchunk y=" + std::to_string(y >> 4) + " was not found [SetFullBlock]");
  }

  subChunk->set(x, y & 0xf, z, block->GetFullId());

  chunk_dirty_ = true;
}

const Block *Chunk::GetFullBlock(int_fast8_t x, int_fast16_t y, int_fast8_t z) {
  SubChunk *subChunk;
  if ((subChunk = GetSubChunk(y >> 4)) == nullptr) {
    throw std::invalid_argument("Subchunk y=" + std::to_string(y >> 4) + " was not found [GetFullBlock]");
  }

  return Block::GetBlockFromFullId(subChunk->get(x, y & 0x0f, z));
}

int_fast16_t Chunk::GetHighestBlockAt(uint_fast8_t x, uint_fast8_t z) {
  for (auto y = block_layer_.size() - 1; y > 0; --y) {
    int_fast16_t height = GetHighestBlockAt(GetSubChunk(y), x, z);
    if (height != -1) {
      return static_cast<int_fast16_t>(height | (y << 4));
    }
  }

  return -1;
}

BiomeArray Chunk::GetBiomeArray() const {
  return biome_array_;
}

int_fast32_t Chunk::GetX() const {
  return chunk_x_;
}

int_fast32_t Chunk::GetZ() const {
  return chunk_z_;
}

void Chunk::SetChunkHash(int64_t chunkHash) {
  int64_t x, z;
  Math::morton2d_decode(chunkHash, x, z);

  chunk_x_ = static_cast<int_fast32_t>(x);
  chunk_z_ = static_cast<int_fast32_t>(z);
}

bool Chunk::IsDirty() const {
  return chunk_dirty_;
}

auto Chunk::IsInUse() const -> bool {
  return chunk_in_use_;
}

void Chunk::SetDirty(bool is_dirty) {
  chunk_dirty_ = is_dirty;
}

void Chunk::SetInUse(bool is_in_use) {
  chunk_in_use_ = is_in_use;
}

void Chunk::Invalidate() {
  chunk_x_ = 0;
  chunk_z_ = 0;
  chunk_dirty_ = false;
  chunk_in_use_ = false;
  block_layer_ = {};

  biome_array_.SetBiomeData(nullptr);
}

int_fast16_t Chunk::GetHighestBlockAt(SubChunk *blocks, int_fast32_t x, int_fast32_t z) {
  for (int_fast16_t y = 15; y >= 0; --y) {
    if (blocks->get(x, y, z) != 0) {
      return y;
    }
  }

  return -1;
}

void AllocationManager::ResetAllocation() {
  while (!c_tracking_.empty()) {
    delete c_tracking_.top();

    c_tracking_.pop();

    allocations_--;
  }

  used_ = 0;
}

void AllocationManager::InitObjects(int default_count) {
  for (int i = default_count; i > 0; i--) {
    auto *chunk = new Chunk();

    c_allocated_.push(chunk);
    c_tracking_.push(chunk);
    allocations_++;
  }
}

Chunk *AllocationManager::GetChunkObject(int64_t) {
  used_++;

  if (c_allocated_.empty()) {
    allocations_++;

    auto *chunk = new Chunk();
    chunk->SetInUse(true);

    c_tracking_.push(chunk);

    return chunk;
  }

  auto *chunk = c_allocated_.top();
  chunk->SetInUse(true);

  c_allocated_.pop();

  return chunk;
}

void AllocationManager::ReturnChunkObject(Chunk *chunk) {
  std::max(used_--, 0);

  chunk->Invalidate();

  c_allocated_.push(chunk);
}

auto AllocationManager::GetObjectAllocations() -> int {
  return allocations_;
}

auto AllocationManager::GetUsedAllocations() -> int {
  return used_;
}