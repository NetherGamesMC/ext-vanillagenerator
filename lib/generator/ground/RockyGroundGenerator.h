#ifndef EXT_NOISELIB_LIB_GENERATOR_GROUND_ROCKYGROUNDGENERATOR_H_
#define EXT_NOISELIB_LIB_GENERATOR_GROUND_ROCKYGROUNDGENERATOR_H_

#include "GroundGenerator.h"

class RockyGroundGenerator : public GroundGenerator {
 public:
  RockyGroundGenerator() {
    top_material = STONE;
    ground_material = STONE;
  }
};

#endif //EXT_NOISELIB_LIB_GENERATOR_GROUND_ROCKYGROUNDGENERATOR_H_
