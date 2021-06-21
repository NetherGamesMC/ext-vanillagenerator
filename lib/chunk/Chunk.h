#ifndef EXT_NOISELIB_CHUNK_H
#define EXT_NOISELIB_CHUNK_H

#include <PhpPalettedBlockArrayObj.h>
#include <lib/objects/BiomeArray.h>

class Chunk {
 public:
  Chunk(int64_t a, std::array<NormalBlockArrayContainer *, 16> &b, BiomeArray *biome_array);

  NormalBlockArrayContainer *getSubChunk(uint_fast8_t y);

  auto setFullBlock(int_fast32_t x, int_fast16_t y, int_fast32_t z, Block block) -> void;
  auto getFullBlock(int_fast32_t x, int_fast16_t y, int_fast32_t z) -> Block;

  auto getHighestBlockAt(uint_fast8_t x, uint_fast8_t z) -> int_fast16_t;

  auto getBiomeArray() const -> BiomeArray *;

  auto getX() const -> int_fast64_t;
  auto getZ() const -> int_fast64_t;

  auto isDirty() const -> bool;
  auto setDirty(bool isDirty) -> void;

  auto destroyObjects() -> void;
 private:
  static int getHighestBlockAt(NormalBlockArrayContainer *blocks, int x, int z);

  std::array<NormalBlockArrayContainer *, 16> blockLayer;
  BiomeArray *biomeArray;

  int_fast64_t chunkX;
  int_fast64_t chunkZ;

  bool chunkDirty;
};

#endif
