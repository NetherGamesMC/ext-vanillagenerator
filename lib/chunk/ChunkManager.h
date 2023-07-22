#pragma once
#ifndef EXT_VANILLAGENERATOR_LIB_CHUNK_CHUNKMANAGER_H_
#define EXT_VANILLAGENERATOR_LIB_CHUNK_CHUNKMANAGER_H_

#include <map>
#include <cstdint>
#include <lib/objects/blocks/Block.h>
#include <lib/chunk/Chunk.h>

class ChunkManager {
 public:
  ChunkManager(const int_fast32_t minHeight, const int_fast32_t maxHeight) : minY(minHeight), maxY(maxHeight) {}

  auto SetChunk(int_fast32_t chunkX, int_fast32_t chunkZ, Chunk *chunk) -> void;
  auto GetChunk(int_fast32_t chunkX, int_fast32_t chunkZ) -> Chunk *;

  auto GetHighestBlockAt(int_fast32_t x, int_fast32_t z) -> const blocks::MCBlock *;
  auto GetBlockAt(int_fast32_t x, int_fast32_t y, int_fast32_t z) -> const blocks::MCBlock *;
  auto SetBlockAt(int_fast32_t x, int_fast32_t y, int_fast32_t z, const blocks::MCBlock *block) -> void;

  auto GetHighestElevationAt(int_fast32_t x, int_fast32_t z) -> int_fast32_t;

  auto IsInWorld(int_fast32_t x, int_fast32_t y, int_fast32_t z) const -> bool;

  auto GetMinY() const -> int_fast32_t;
  auto GetMaxY() const -> int_fast32_t;

  auto GetChunks() const -> std::map<uint_fast64_t, Chunk *>;

 private:
  std::map<uint_fast64_t, Chunk *> chunks;

  int_fast32_t minY;
  int_fast32_t maxY;
};

#endif // EXT_VANILLAGENERATOR_LIB_CHUNK_CHUNKMANAGER_H_
