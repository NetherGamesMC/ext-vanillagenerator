#ifndef EXT_VANILLAGENERATOR_LIB_GENERATOR_OBJECTS_TALLGRASS_H_
#define EXT_VANILLAGENERATOR_LIB_GENERATOR_OBJECTS_TALLGRASS_H_

#include "TerrainObjects.h"

class TallGrass : public TerrainObjects {
 public:
  explicit TallGrass(MinecraftBlock grassType) : grassType_(grassType) {}

  bool Generate(ChunkManager &world,
                Random &random,
                int_fast32_t sourceX,
                int_fast32_t sourceY,
                int_fast32_t sourceZ) override;

 private:
  MinecraftBlock grassType_;
};

#endif //EXT_VANILLAGENERATOR_LIB_GENERATOR_OBJECTS_TALLGRASS_H_
