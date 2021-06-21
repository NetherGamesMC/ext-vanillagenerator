#include <lib/MortonHelper.h>
#include <lib/chunk/SimpleChunkManager.h>

Chunk *SimpleChunkManager::getChunk(int64_t chunkX, int64_t chunkZ) {
  uint_fast64_t location = morton2d_encode(chunkX, chunkZ);

  auto searchResult = chunks.find(location);
  if (searchResult == chunks.end()) {
    return nullptr;
  }

  return searchResult->second;
}

void SimpleChunkManager::setChunk(int64_t chunkX, int64_t chunkZ, Chunk *chunk) {
  chunks.insert({morton2d_encode(chunkX, chunkZ), chunk});
}

MinecraftBlock SimpleChunkManager::getBlockAt(int x, int y, int z) {
  Chunk *chunk;
  if (isInWorld(x, y, z) && (chunk = getChunk(x >> 4, z >> 4)) != nullptr) {
    return MinecraftBlock(chunk->getFullBlock(x & 0xf, y, z & 0xf));
  }

  return MinecraftBlock((Block) 0);
}

void SimpleChunkManager::setBlockAt(int x, int y, int z, MinecraftBlock block) {
  Chunk *chunk;

  if ((chunk = getChunk(x >> 4, z >> 4)) != nullptr) {
    chunk->setFullBlock(x & 0xf, y, z & 0xf, block.getFullId());
  } else {
    throw std::invalid_argument(
        "Cannot set block at coordinates x=" + std::to_string(x) + ", y=" + std::to_string(y) + ", z="
            + std::to_string(z) + ", terrain is not loaded or out of bounds");
  }
}

bool SimpleChunkManager::isInWorld(int x, int y, int z) const {
  return x <= INT32_MAX && x >= INT32_MIN && y < maxY && y >= minY && z <= INT32_MAX && z >= INT32_MIN;
}

int_fast16_t SimpleChunkManager::getMinY() const {
  return minY;
}

int_fast16_t SimpleChunkManager::getMaxY() const {
  return maxY;
}

void SimpleChunkManager::destroyObjects() {
  for (auto data : chunks) {
    data.second->destroyObjects();

    delete data.second;
  }

  chunks.clear();
}

std::map<uint_fast64_t, Chunk *> SimpleChunkManager::getChunks() const {
  return chunks;
}