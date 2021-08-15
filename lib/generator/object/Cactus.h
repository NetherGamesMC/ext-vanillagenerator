#ifndef EXT_VANILLAGENERATOR_LIB_GENERATOR_OBJECTS_CACTUS_H_
#define EXT_VANILLAGENERATOR_LIB_GENERATOR_OBJECTS_CACTUS_H_

#include <lib/objects/math/Facing.h>
#include "TerrainObjects.h"

class Cactus {
 public:
  static bool Generate(ChunkManager &world, Random &random, int_fast32_t sourceX, int_fast16_t sourceY, int_fast32_t sourceZ);
};

#endif //EXT_VANILLAGENERATOR_LIB_GENERATOR_OBJECTS_CACTUS_H_
