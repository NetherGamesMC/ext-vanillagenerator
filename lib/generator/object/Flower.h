#ifndef EXT_VANILLAGENERATOR_LIB_GENERATOR_OBJECTS_FLOWER_H_
#define EXT_VANILLAGENERATOR_LIB_GENERATOR_OBJECTS_FLOWER_H_

#include "TerrainObjects.h"

using namespace blocks;

class Flower : public TerrainObjects {
 public:
  explicit Flower(const MCBlock *block) : type(block) {}

  bool Generate(ChunkManager &world, Random &random, int_fast32_t sourceX, int_fast32_t sourceY, int_fast32_t sourceZ) override;
 private:
  const MCBlock *type;
};

#endif //EXT_VANILLAGENERATOR_LIB_GENERATOR_OBJECTS_FLOWER_H_
