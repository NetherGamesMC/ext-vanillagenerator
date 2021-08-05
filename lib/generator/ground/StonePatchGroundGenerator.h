#ifndef EXT_NOISELIB_LIB_GENERATOR_GROUND_STONEPATCHGROUNDGENERATOR_H_
#define EXT_NOISELIB_LIB_GENERATOR_GROUND_STONEPATCHGROUNDGENERATOR_H_

#include "GroundGenerator.h"

class StonePatchGroundGenerator : public GroundGenerator {
 public:
  void GenerateTerrainColumn(ChunkManager &world,
                             Random &random,
                             int_fast64_t x,
                             int_fast64_t z,
                             int biome,
                             double surface_noise) override;
};

#endif //EXT_NOISELIB_LIB_GENERATOR_GROUND_STONEPATCHGROUNDGENERATOR_H_
