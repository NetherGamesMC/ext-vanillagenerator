#ifndef EXT_NOISELIB_LIB_GENERATOR_GROUND_DIRTPATCHGROUNDGENERATOR_H_
#define EXT_NOISELIB_LIB_GENERATOR_GROUND_DIRTPATCHGROUNDGENERATOR_H_

#include "GroundGenerator.h"

class DirtPatchGroundGenerator : public GroundGenerator {
 public:
  void GenerateTerrainColumn(ChunkManager &world,
                             Random &random,
                             int_fast64_t x,
                             int_fast64_t z,
                             int biome,
                             double surfaceNoise) override;
};

#endif //EXT_NOISELIB_LIB_GENERATOR_GROUND_DIRTPATCHGROUNDGENERATOR_H_
