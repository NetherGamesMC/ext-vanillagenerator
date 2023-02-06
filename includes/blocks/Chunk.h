#ifndef VANILLAGENERATOR_SRC_BLOCKS_CHUNK_H_
#define VANILLAGENERATOR_SRC_BLOCKS_CHUNK_H_

#include <map>
#include <BlockArrayContainer.h>
#include <stack>
#include "Blocks.h"

using namespace block;

namespace chunk {

class Chunk;
class ChunkManager;
class BiomeArray;
class AllocationManager;

const size_t BIOME_ARRAY_SIZE = 256;

typedef BlockArrayContainer<unsigned int> SubChunk;
typedef gsl::span<uint_fast8_t, BIOME_ARRAY_SIZE> BiomeList;
typedef std::array<SubChunk *, 16> SubChunkList;
typedef std::map<uint_fast64_t, Chunk *> ChunkArray;

class BiomeArray {
 public:
  [[nodiscard]] auto Get(uint8_t x, uint8_t z) const -> uint_fast8_t;
  [[nodiscard]] auto GetRawData() const -> BiomeList *;

  auto Set(uint8_t x, uint8_t z, uint_fast8_t value) -> void;
  auto SetBiomeData(BiomeList *) -> void;
 private:
  BiomeList *biomes_;

  static inline void Index(uint8_t x, uint8_t z, uint_fast8_t &offset);
};

class Chunk {
 public:
  Chunk();

  SubChunk *GetSubChunk(uint_fast8_t y);

  auto SetFullBlock(int_fast8_t x, int_fast16_t y, int_fast8_t z, const Block *block) -> void;
  auto GetFullBlock(int_fast8_t x, int_fast16_t y, int_fast8_t z) -> const Block *;

  auto GetHighestBlockAt(uint_fast8_t x, uint_fast8_t z) -> int_fast16_t;

  [[nodiscard]] auto GetBiomeArray() const -> BiomeArray;

  [[nodiscard]] auto GetX() const -> int_fast32_t;
  [[nodiscard]] auto GetZ() const -> int_fast32_t;

  auto SetChunkHash(int64_t) -> void;

  [[nodiscard]] auto IsDirty() const -> bool;
  [[nodiscard]] auto IsInUse() const -> bool;

  auto SetDirty(bool is_dirty) -> void;
  auto SetInUse(bool is_in_use) -> void;

  auto Invalidate() -> void;
 private:
  static int_fast16_t GetHighestBlockAt(SubChunk *blocks, int_fast32_t x, int_fast32_t z);

  SubChunkList block_layer_;
  BiomeArray biome_array_;

  int_fast32_t chunk_x_ = 0;
  int_fast32_t chunk_z_ = 0;

  bool chunk_dirty_ = false;
  bool chunk_in_use_ = false;
};

class ChunkManager {
 public:
  ChunkManager(int_fast32_t min_height, int_fast32_t max_height);
  ~ChunkManager();

  auto SetChunk(int_fast64_t chunk_x, int_fast64_t chunk_z, Chunk *chunk) -> void;
  auto GetChunk(int_fast64_t chunk_x, int_fast64_t chunk_z) -> Chunk *;

  auto GetHighestBlockAt(int_fast32_t x, int_fast32_t z) -> const Block *;
  auto GetBlockAt(int_fast32_t x, int_fast32_t y, int_fast32_t z) -> const Block *;
  auto SetBlockAt(int_fast32_t x, int_fast32_t y, int_fast32_t z, const Block *block) -> void;

  auto GetHighestElevationAt(int_fast32_t x, int_fast32_t z) -> int_fast32_t;

  [[nodiscard]] auto IsInWorld(int_fast32_t x, int_fast32_t y, int_fast32_t z) const -> bool;

  [[nodiscard]] auto GetMinY() const -> int_fast32_t;
  [[nodiscard]] auto GetMaxY() const -> int_fast32_t;

  [[nodiscard]] auto GetChunks() const -> std::map<uint_fast64_t, Chunk *>;
 private:
  ChunkArray chunks_;

  int_fast32_t min_y_;
  int_fast32_t max_y_;
};

/**
 * Responsible in creating and managing all pointers instantiated here.
 * In order to reduce the number of objects created for each "Chunk", the object(s) will be stored in a stack. Default
 * initialization will be returned from the stack and once the object is no longer in use, it will be returned to stack.
 *
 * !! Returning an object to the allocation manager must be done manually !!
 */
class AllocationManager {
 public:
  static void ResetAllocation();
  static void InitObjects(int default_count);

  static Chunk *GetChunkObject(int64_t);
  static void ReturnChunkObject(Chunk *);

  [[nodiscard]] static auto GetObjectAllocations() -> int;
  [[nodiscard]] static auto GetUsedAllocations() -> int;
 private:
  // Allocation statistics.
  inline static int allocations_ = 0;
  inline static int used_ = 0;
  inline static std::stack<Chunk *> c_allocated_;
  inline static std::stack<Chunk *> c_tracking_;
};

}

#endif // VANILLAGENERATOR_SRC_BLOCKS_CHUNK_H_
