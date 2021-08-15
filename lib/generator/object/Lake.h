#ifndef EXT_VANILLAGENERATOR_LIB_GENERATOR_OBJECTS_LAKE_H_
#define EXT_VANILLAGENERATOR_LIB_GENERATOR_OBJECTS_LAKE_H_

#include <lib/generator/misc/BlockValidators.h>

#include <utility>
#include "TerrainObjects.h"

class Lake : public TerrainObjects {
 public:
  explicit Lake(MinecraftBlock block, BlockTransaction &transaction);

  bool Generate(ChunkManager &world, Random &random, int_fast32_t sourceX, int_fast32_t sourceY, int_fast32_t sourceZ) override;

 private:
  static const int_fast32_t LAKE_MAX_HEIGHT;
  static const int_fast32_t LAKE_MAX_DIAMETER;

  auto CanPlace(std::vector<int_fast64_t> &lakeMap, ChunkManager &world, int_fast32_t sourceX, int_fast32_t sourceY, int_fast32_t sourceZ) -> bool;

  static auto IsLakeBlock(std::vector<int_fast64_t> &lakeMap, int_fast32_t x, int_fast32_t y, int_fast32_t z) -> bool;

  static auto SetLakeBlock(std::vector<int_fast64_t> &lakeMap, int_fast32_t x, int_fast32_t y, int_fast32_t z) -> void;

  MinecraftBlock type_;
  BlockTransaction &transaction_;
};

#endif //EXT_VANILLAGENERATOR_LIB_GENERATOR_OBJECTS_LAKE_H_
