#ifndef EXT_NOISELIB_LIB_CHUNK_CHUNKMANAGER_H_
#define EXT_NOISELIB_LIB_CHUNK_CHUNKMANAGER_H_

#include <map>
#include <cstdint>
#include <lib/objects/MinecraftBlock.h>
#include <lib/chunk/Chunk.h>

class ChunkManager {
 public:
  ChunkManager(const int_fast16_t minHeight, const int_fast16_t maxHeight) : minY(minHeight), maxY(maxHeight) {}
  ~ChunkManager();

  auto SetChunk(int64_t chunkX, int64_t chunkZ, Chunk *chunk) -> void;
  auto GetChunk(int64_t chunkX, int64_t chunkZ) -> Chunk *;

  auto GetHighestBlockAt(int_fast64_t x, int_fast64_t z) -> MinecraftBlock;
  auto GetBlockAt(int_fast64_t x, int_fast16_t y, int_fast64_t z) -> MinecraftBlock;
  auto SetBlockAt(int_fast64_t x, int_fast16_t y, int_fast64_t z, MinecraftBlock block) -> void;

  auto GetHighestElevationAt(int_fast64_t x, int_fast64_t z) -> int_fast16_t;

  [[nodiscard]] auto IsInWorld(int_fast64_t x, int_fast16_t y, int_fast64_t z) const -> bool;

  [[nodiscard]] auto GetMinY() const -> int_fast16_t;
  [[nodiscard]] auto GetMaxY() const -> int_fast16_t;

  [[nodiscard]] auto GetChunks() const -> std::map<uint_fast64_t, Chunk *>;

 private:
  std::map<uint_fast64_t, Chunk *> chunks;

  int_fast16_t minY;
  int_fast16_t maxY;
};

#endif // EXT_NOISELIB_LIB_CHUNK_CHUNKMANAGER_H_
