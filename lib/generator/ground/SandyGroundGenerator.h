#ifndef EXT_NOISELIB_LIB_GENERATOR_GROUND_SANDYGROUNDGENERATOR_H_
#define EXT_NOISELIB_LIB_GENERATOR_GROUND_SANDYGROUNDGENERATOR_H_

#include "GroundGenerator.h"

class SandyGroundGenerator : public GroundGenerator {
 public:
  SandyGroundGenerator() {
    top_material = SAND;
    ground_material = SAND;
  }
};

#endif //EXT_NOISELIB_LIB_GENERATOR_GROUND_SANDYGROUNDGENERATOR_H_
