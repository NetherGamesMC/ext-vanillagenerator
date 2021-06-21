#ifndef EXT_NOISELIB_LIB_GENERATOR_OBJECTS_DOUBLETALLPLANT_H_
#define EXT_NOISELIB_LIB_GENERATOR_OBJECTS_DOUBLETALLPLANT_H_

#include "TerrainObjects.h"

class DoubleTallPlant : public TerrainObjects {
 public:
  explicit DoubleTallPlant(MinecraftBlock subtype) : species(subtype) {}

  bool generate(SimpleChunkManager world, Random &random, int source_x, int source_y, int source_z) override;
 private:
  MinecraftBlock species;
};

#endif //EXT_NOISELIB_LIB_GENERATOR_OBJECTS_DOUBLETALLPLANT_H_
