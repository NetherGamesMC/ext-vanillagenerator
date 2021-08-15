#ifndef EXT_VANILLAGENERATOR_LIB_GENERATOR_GROUND_GROUNDGENERATOR_H_
#define EXT_VANILLAGENERATOR_LIB_GENERATOR_GROUND_GROUNDGENERATOR_H_

#include <lib/objects/constants/BlockList.h>
#include <lib/chunk/ChunkManager.h>
#include <lib/objects/random/Random.h>

class GroundGenerator {
 public:
  virtual void GenerateTerrainColumn(ChunkManager &world,
                                     Random &random,
                                     int_fast32_t x,
                                     int_fast32_t z,
                                     int biome,
                                     double surfaceNoise);
 protected:

  MinecraftBlock topMaterial = GRASS;
  MinecraftBlock groundMaterial = DIRT;
};

#endif // EXT_VANILLAGENERATOR_LIB_GENERATOR_GROUND_GROUNDGENERATOR_H_
