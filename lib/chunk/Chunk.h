#ifndef EXT_VANILLAGENERATOR_CHUNK_H
#define EXT_VANILLAGENERATOR_CHUNK_H

#include <PhpPalettedBlockArrayObj.h>
#include <lib/objects/math/Math.h>

class Chunk;
class MCBiomeArray;

class Chunk {
 public:
  static const int Y_MIN = -64;
  static const int Y_MAX = 320;
  static const int MIN_SUBCHUNK_INDEX = -4;
  static const int MAX_SUBCHUNK_INDEX = 19;
  static const int MAX_SUBCHUNKS = Chunk::MAX_SUBCHUNK_INDEX - Chunk::MIN_SUBCHUNK_INDEX + 1;

  typedef std::array<NormalBlockArrayContainer *, Chunk::MAX_SUBCHUNKS> BlockContainer;

  Chunk(uint_fast64_t chunk, BlockContainer &b, MCBiomeArray &biomeArray);

  NormalBlockArrayContainer *GetSubChunk(int_fast8_t y);

  auto SetFullBlock(int_fast8_t x, int_fast16_t y, int_fast8_t z, Block block) -> void;
  auto GetFullBlock(int_fast8_t x, int_fast16_t y, int_fast8_t z) -> Block;

  auto GetHighestBlockAt(uint_fast8_t x, uint_fast8_t z) -> int_fast16_t;

  [[nodiscard]] auto GetBiomeArray() const -> MCBiomeArray &;

  [[nodiscard]] auto GetX() const -> int_fast32_t;
  [[nodiscard]] auto GetZ() const -> int_fast32_t;

  [[nodiscard]] auto IsDirty() const -> bool;

  auto SetDirty(bool isDirty) -> void;
 private:
  static int_fast16_t GetHighestBlockAt(NormalBlockArrayContainer *blocks, int_fast32_t x, int_fast32_t z);

  BlockContainer &blockLayer;
  MCBiomeArray &biomeArray;

  int_fast32_t chunkX = 0;
  int_fast32_t chunkZ = 0;

  bool chunkDirty = false;
};

typedef Chunk::BlockContainer BlockContainer;

class MCBiomeArray {
 public:
  MCBiomeArray(BlockContainer &b);

  auto Get(uint8_t x, uint8_t z) const -> uint_fast8_t;
  auto Set(uint8_t x, uint8_t z, uint_fast8_t value) -> void;
 private:
  BlockContainer &mValues;
};

#endif
