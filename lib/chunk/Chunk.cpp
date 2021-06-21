#include "Chunk.h"

#include <lib/MortonHelper.h>

Chunk::Chunk(int64_t a, std::array<NormalBlockArrayContainer *, 16> &b, BiomeArray *biome_array) : biomeArray(
    biome_array) {
  std::copy(std::begin(b), std::end(b), std::begin(blockLayer));

  morton2d_decode(a, chunkX, chunkZ);
}

NormalBlockArrayContainer *Chunk::getSubChunk(uint_fast8_t y) {
  if (y < 0 || y >= blockLayer.size()) {
    return nullptr;
  }

  return blockLayer[y];
}

void Chunk::setFullBlock(int_fast32_t x, int_fast16_t y, int_fast32_t z, Block block) {
  NormalBlockArrayContainer *subChunk;
  if ((subChunk = getSubChunk(y >> 4)) == nullptr) {
    throw std::invalid_argument("Subchunk y=" + std::to_string(y >> 4) + " were not found");
  }

  subChunk->set(x, y & 0xf, z, block);

  chunkDirty = true;
}

Block Chunk::getFullBlock(int_fast32_t x, int_fast16_t y, int_fast32_t z) {
  BlockArrayContainer<Block> *subChunk;
  if ((subChunk = getSubChunk(y >> 4)) == nullptr) {
    throw std::invalid_argument("Subchunk y=" + std::to_string(y >> 4) + " were not found");
  }

  return subChunk->get(x, y & 0x0f, z);
}

int_fast16_t Chunk::getHighestBlockAt(uint_fast8_t x, uint_fast8_t z) {
  for (auto y = static_cast<int_fast16_t>(blockLayer.size() - 1); y >= 0; --y) {
    int height = getHighestBlockAt(getSubChunk(y), x, z);
    if (height != -1) {
      return height | (y << 4);
    }
  }

  return -1;
}

BiomeArray *Chunk::getBiomeArray() const {
  return biomeArray;
}

int_fast64_t Chunk::getX() const {
  return chunkX;
}

int_fast64_t Chunk::getZ() const {
  return chunkZ;
}

bool Chunk::isDirty() const {
  return chunkDirty;
}

void Chunk::setDirty(bool isDirty) {
  chunkDirty = isDirty;
}

int Chunk::getHighestBlockAt(NormalBlockArrayContainer *blocks, int x, int z) {
  for (int y = 15; y >= 0; --y) {
    if (blocks->get(x, y, z) != 0) {
      return y;
    }
  }

  return -1;
}

void Chunk::destroyObjects() {
  delete biomeArray;
}
