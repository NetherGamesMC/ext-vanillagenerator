#ifndef EXT_NOISELIB_LIB_GENERATOR_OBJECTS_SUGARCANE_H_
#define EXT_NOISELIB_LIB_GENERATOR_OBJECTS_SUGARCANE_H_

#include "TerrainObjects.h"

class SugarCane : public TerrainObjects {
  bool Generate(ChunkManager world,
                Random &random,
                int_fast64_t sourceX,
                int_fast32_t sourceY,
                int_fast64_t sourceZ) override;
};

#endif //EXT_NOISELIB_LIB_GENERATOR_OBJECTS_SUGARCANE_H_
