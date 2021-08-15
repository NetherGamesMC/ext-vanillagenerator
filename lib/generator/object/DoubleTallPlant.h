#ifndef EXT_VANILLAGENERATOR_LIB_GENERATOR_OBJECTS_DOUBLETALLPLANT_H_
#define EXT_VANILLAGENERATOR_LIB_GENERATOR_OBJECTS_DOUBLETALLPLANT_H_

#include "TerrainObjects.h"

class DoubleTallPlant : public TerrainObjects {
 public:
  explicit DoubleTallPlant(MinecraftBlock subtype) : species_(subtype) {}

  bool Generate(ChunkManager &world,
                Random &random,
                int_fast32_t sourceX,
                int_fast32_t sourceY,
                int_fast32_t sourceZ) override;

 private:
  MinecraftBlock species_;
};

#endif //EXT_VANILLAGENERATOR_LIB_GENERATOR_OBJECTS_DOUBLETALLPLANT_H_
