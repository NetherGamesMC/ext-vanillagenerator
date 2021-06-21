#ifndef EXT_NOISELIB_LIB_GENERATOR_OBJECTS_LAKE_H_
#define EXT_NOISELIB_LIB_GENERATOR_OBJECTS_LAKE_H_

#include "TerrainObjects.h"

class Lake : public TerrainObjects {
 public:
  explicit Lake(MinecraftBlock block) : type(block) {
    // NOOP
  }

  bool generate(SimpleChunkManager world, Random &random, int source_x, int source_y, int source_z) override;

 private:
  constexpr static const int LAKE_MAX_HEIGHT = 8;
  constexpr static const int LAKE_MAX_DIAMETER = 16;

  static bool isLakeBlock(std::vector<int32_t> &lake_map, int x, int y, int z);

  static void setLakeBlock(std::vector<int32_t> &lake_map, int x, int y, int z);

  bool canPlace(std::vector<int32_t> &lake_map, SimpleChunkManager world, int sourceX, int sourceY, int sourceZ);

  MinecraftBlock type;
};

#endif //EXT_NOISELIB_LIB_GENERATOR_OBJECTS_LAKE_H_
