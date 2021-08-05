#ifndef EXT_NOISELIB_LIB_GENERATOR_OBJECTS_FLOWER_H_
#define EXT_NOISELIB_LIB_GENERATOR_OBJECTS_FLOWER_H_

#include "TerrainObjects.h"

class Flower : public TerrainObjects {
 public:
  explicit Flower(MinecraftBlock block) : type_(block) {}

  bool Generate(ChunkManager world,
                Random &random,
                int_fast64_t sourceX,
                int_fast32_t sourceY,
                int_fast64_t sourceZ) override;

 private:
  MinecraftBlock type_;
};

#endif //EXT_NOISELIB_LIB_GENERATOR_OBJECTS_FLOWER_H_
