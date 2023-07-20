#include "Chunk.h"

#include <lib/MortonHelper.h>

Chunk::Chunk(int64_t chunk, std::array<NormalBlockArrayContainer *, Chunk::MAX_SUBCHUNKS> &b, MCBiomeArray &biomeArray) : biomeArray(biomeArray), blockLayer(b) {
  int64_t x, z;
  morton2d_decode(chunk, x, z);

  chunkX = static_cast<int_fast32_t>(x);
  chunkZ = static_cast<int_fast32_t>(z);
}

NormalBlockArrayContainer *Chunk::GetSubChunk(int_fast8_t y) {
  if (y < Chunk::MIN_SUBCHUNK_INDEX || y >= Chunk::MAX_SUBCHUNK_INDEX) {
    return nullptr;
  }

  return blockLayer[y - Chunk::MIN_SUBCHUNK_INDEX];
}

void Chunk::SetFullBlock(int_fast8_t x, int_fast16_t y, int_fast8_t z, Block block) {
  NormalBlockArrayContainer *subChunk;
  if ((subChunk = GetSubChunk(y >> 4)) == nullptr) {
    throw std::invalid_argument("Subchunk y=" + std::to_string(y >> 4) + " was not found [SetFullBlock]");
  }

  subChunk->set(x, y & 0xf, z, block);

  chunkDirty = true;
}

Block Chunk::GetFullBlock(int_fast8_t x, int_fast16_t y, int_fast8_t z) {
  BlockArrayContainer<Block> *subChunk;
  if ((subChunk = GetSubChunk(y >> 4)) == nullptr) {
    throw std::invalid_argument("Subchunk y=" + std::to_string(y >> 4) + " was not found [GetFullBlock]");
  }

  return subChunk->get(x, y & 0x0f, z);
}

int_fast16_t Chunk::GetHighestBlockAt(uint_fast8_t x, uint_fast8_t z) {
  for (auto y = static_cast<int_fast16_t>(blockLayer.size() - 1); y >= 0; --y) {
    int_fast16_t height = GetHighestBlockAt(GetSubChunk(y), x, z);
    if (height > Y_MIN - 1) {
      return static_cast<int_fast16_t>(height | (y << 4));
    }
  }

  return Y_MIN - 1;
}

MCBiomeArray &Chunk::GetBiomeArray() const {
  return biomeArray;
}

int_fast32_t Chunk::GetX() const {
  return chunkX;
}

int_fast32_t Chunk::GetZ() const {
  return chunkZ;
}

bool Chunk::IsDirty() const {
  return chunkDirty;
}

void Chunk::SetDirty(bool isDirty) {
  chunkDirty = isDirty;
}

int_fast16_t Chunk::GetHighestBlockAt(NormalBlockArrayContainer *blocks, int_fast32_t x, int_fast32_t z) {
  for (int_fast16_t y = 15; y >= 0; --y) {
    if (blocks->get(x, y, z) != 0) {
      return y;
    }
  }

  return Y_MIN - 1;
}

MCBiomeArray::MCBiomeArray(std::array<NormalBlockArrayContainer *, Chunk::MAX_SUBCHUNKS> &values): mValues(values) {
}

uint8_t MCBiomeArray::Get(const uint8_t x, const uint8_t z) const {
  return mValues[0]->get(x, 0, z);
}

void MCBiomeArray::Set(const uint8_t x, const uint8_t z, const uint8_t value) {
  for (auto biome : mValues){
    for (int y = 15; y >= 0; --y) {
      biome->set(x, y, z, value);
    }
  }
}