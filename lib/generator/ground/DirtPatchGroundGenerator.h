#ifndef EXT_VANILLAGENERATOR_LIB_GENERATOR_GROUND_DIRTPATCHGROUNDGENERATOR_H_
#define EXT_VANILLAGENERATOR_LIB_GENERATOR_GROUND_DIRTPATCHGROUNDGENERATOR_H_

#include "GroundGenerator.h"

class DirtPatchGroundGenerator : public GroundGenerator {
 public:
  void GenerateTerrainColumn(ChunkManager &world,
                             Random &random,
                             int_fast32_t x,
                             int_fast32_t z,
                             int biome,
                             double surfaceNoise) override;
};

#endif //EXT_VANILLAGENERATOR_LIB_GENERATOR_GROUND_DIRTPATCHGROUNDGENERATOR_H_
