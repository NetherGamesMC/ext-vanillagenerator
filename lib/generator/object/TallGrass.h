#ifndef EXT_VANILLAGENERATOR_LIB_GENERATOR_OBJECTS_TALLGRASS_H_
#define EXT_VANILLAGENERATOR_LIB_GENERATOR_OBJECTS_TALLGRASS_H_

#include "TerrainObjects.h"

using namespace blocks;

class TallGrass : public TerrainObjects {
 public:
  explicit TallGrass(const MCBlock *grassType) : grassType_(grassType) {}

  bool Generate(ChunkManager &world, Random &random, int_fast32_t sourceX, int_fast32_t sourceY, int_fast32_t sourceZ) override;
 private:
  const MCBlock *grassType_;
};

#endif //EXT_VANILLAGENERATOR_LIB_GENERATOR_OBJECTS_TALLGRASS_H_
