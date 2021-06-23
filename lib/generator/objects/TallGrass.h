#ifndef EXT_NOISELIB_LIB_GENERATOR_OBJECTS_TALLGRASS_H_
#define EXT_NOISELIB_LIB_GENERATOR_OBJECTS_TALLGRASS_H_

#include "TerrainObjects.h"

class TallGrass : public TerrainObjects {
 public:
  explicit TallGrass(MinecraftBlock grassType): grassType_(grassType) {}

  bool generate(SimpleChunkManager world, Random &random, int source_x, int source_y, int source_z) override;

 private:
  MinecraftBlock grassType_;
};

#endif //EXT_NOISELIB_LIB_GENERATOR_OBJECTS_TALLGRASS_H_
