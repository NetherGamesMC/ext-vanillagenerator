#ifndef EXT_NOISELIB_CHUNK_H
#define EXT_NOISELIB_CHUNK_H

#include <PhpPalettedBlockArrayObj.h>
#include <lib/biomes/BiomeArray.h>

class Chunk {
 public:
  Chunk(int64_t chunk, std::array<NormalBlockArrayContainer *, 16> &b, BiomeArray *biome_array);

  NormalBlockArrayContainer *GetSubChunk(uint_fast8_t y);

  auto SetFullBlock(int_fast8_t x, int_fast16_t y, int_fast8_t z, Block block) -> void;
  auto GetFullBlock(int_fast8_t x, int_fast16_t y, int_fast8_t z) -> Block;

  auto GetHighestBlockAt(uint_fast8_t x, uint_fast8_t z) -> int_fast16_t;

  [[nodiscard]] auto GetBiomeArray() const -> BiomeArray *;

  [[nodiscard]] auto GetX() const -> int_fast32_t;
  [[nodiscard]] auto GetZ() const -> int_fast32_t;

  [[nodiscard]] auto IsDirty() const -> bool;

  auto SetDirty(bool isDirty) -> void;

  auto DestroyObjects() -> void;
 private:
  static int_fast16_t GetHighestBlockAt(NormalBlockArrayContainer *blocks, int x, int z);

  std::array<NormalBlockArrayContainer *, 16> blockLayer = {};
  BiomeArray *biomeArray;

  int_fast32_t chunkX = 0;
  int_fast32_t chunkZ = 0;

  bool chunkDirty = false;
};

#endif
