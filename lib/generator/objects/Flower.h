#ifndef EXT_NOISELIB_LIB_GENERATOR_OBJECTS_FLOWER_H_
#define EXT_NOISELIB_LIB_GENERATOR_OBJECTS_FLOWER_H_

#include "TerrainObjects.h"

class Flower : public TerrainObjects {
 public:
  explicit Flower(MinecraftBlock block) : type(block) {}

  bool generate(SimpleChunkManager world, Random &random, int source_x, int source_y, int source_z) override;

 private:
  MinecraftBlock type;
};

#endif //EXT_NOISELIB_LIB_GENERATOR_OBJECTS_FLOWER_H_
