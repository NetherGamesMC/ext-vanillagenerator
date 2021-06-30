#ifndef EXT_NOISELIB_LIB_GENERATOR_OBJECTS_DOUBLETALLPLANT_H_
#define EXT_NOISELIB_LIB_GENERATOR_OBJECTS_DOUBLETALLPLANT_H_

#include "TerrainObjects.h"

class DoubleTallPlant : public TerrainObjects {
 public:
  explicit DoubleTallPlant(MinecraftBlock subtype) : species_(subtype) {}

  bool Generate(SimpleChunkManager world,
                Random &random,
                int_fast64_t sourceX,
                int_fast32_t sourceY,
                int_fast64_t sourceZ) override;

 private:
  MinecraftBlock species_;
};

#endif //EXT_NOISELIB_LIB_GENERATOR_OBJECTS_DOUBLETALLPLANT_H_
