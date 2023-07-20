#ifndef EXT_VANILLAGENERATOR_LIB_GENERATOR_OBJECTS_DOUBLETALLPLANT_H_
#define EXT_VANILLAGENERATOR_LIB_GENERATOR_OBJECTS_DOUBLETALLPLANT_H_

#include "TerrainObjects.h"

using namespace blocks;

class DoubleTallPlant : public TerrainObjects {
 public:
  explicit DoubleTallPlant(const MCBlock *subtype) : species(subtype) {}

  bool Generate(ChunkManager &world, Random &random, int_fast32_t sourceX, int_fast32_t sourceY, int_fast32_t sourceZ) override;
 private:
  const MCBlock *species;
};

#endif //EXT_VANILLAGENERATOR_LIB_GENERATOR_OBJECTS_DOUBLETALLPLANT_H_
