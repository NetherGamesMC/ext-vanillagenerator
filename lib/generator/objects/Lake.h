#ifndef EXT_NOISELIB_LIB_GENERATOR_OBJECTS_LAKE_H_
#define EXT_NOISELIB_LIB_GENERATOR_OBJECTS_LAKE_H_

#include <lib/generator/misc/BlockValidators.h>

#include <utility>
#include "TerrainObjects.h"

class Lake : public TerrainObjects {
 public:
  explicit Lake(MinecraftBlock block, BlockTransaction &transaction)
      : type_(block), transaction_(transaction) {}

  bool Generate(ChunkManager world,
                Random &random,
                int_fast64_t sourceX,
                int_fast32_t sourceY,
                int_fast64_t sourceZ) override;

 private:
  constexpr static const int LAKE_MAX_HEIGHT = 8;
  constexpr static const int LAKE_MAX_DIAMETER = 16;

  auto CanPlace(std::vector<int_fast64_t> &lakeMap,
                ChunkManager world,
                int_fast64_t sourceX,
                int_fast32_t sourceY,
                int_fast64_t sourceZ) -> bool;

  static auto IsLakeBlock(std::vector<int_fast64_t> &lakeMap, int_fast64_t x, int_fast32_t y, int_fast64_t z) -> bool;

  static auto SetLakeBlock(std::vector<int_fast64_t> &lakeMap, int_fast64_t x, int_fast32_t y, int_fast64_t z) -> void;

  MinecraftBlock type_;
  BlockTransaction &transaction_;
};

#endif //EXT_NOISELIB_LIB_GENERATOR_OBJECTS_LAKE_H_
