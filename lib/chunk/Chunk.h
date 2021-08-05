#ifndef EXT_NOISELIB_CHUNK_H
#define EXT_NOISELIB_CHUNK_H

#include <PhpPalettedBlockArrayObj.h>
#include <lib/biomes/BiomeArray.h>

class Chunk {
 public:
  Chunk(int64_t chunk, std::array<NormalBlockArrayContainer *, 16> &b, BiomeArray *biome_array);

  NormalBlockArrayContainer *getSubChunk(uint_fast8_t y);

  auto setFullBlock(int_fast8_t x, int_fast16_t y, int_fast8_t z, Block block) -> void;
  auto getFullBlock(int_fast8_t x, int_fast16_t y, int_fast8_t z) -> Block;

  auto getHighestBlockAt(uint_fast8_t x, uint_fast8_t z) -> int_fast16_t;

  [[nodiscard]] auto getBiomeArray() const -> BiomeArray *;

  [[nodiscard]] auto getX() const -> int_fast64_t;
  [[nodiscard]] auto getZ() const -> int_fast64_t;

  [[nodiscard]] auto isDirty() const -> bool;

  auto setDirty(bool isDirty) -> void;

  auto destroyObjects() -> void;
 private:
  static int_fast16_t getHighestBlockAt(NormalBlockArrayContainer *blocks, int x, int z);

  std::array<NormalBlockArrayContainer *, 16> blockLayer = {};
  BiomeArray *biomeArray;

  int_fast64_t chunkX = 0;
  int_fast64_t chunkZ = 0;

  bool chunkDirty = false;
};

#endif
