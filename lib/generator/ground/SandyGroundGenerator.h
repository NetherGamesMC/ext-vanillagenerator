#ifndef EXT_VANILLAGENERATOR_LIB_GENERATOR_GROUND_SANDYGROUNDGENERATOR_H_
#define EXT_VANILLAGENERATOR_LIB_GENERATOR_GROUND_SANDYGROUNDGENERATOR_H_

#include "GroundGenerator.h"

class SandyGroundGenerator : public GroundGenerator {
 public:
  SandyGroundGenerator() {
    topMaterial = SAND;
    groundMaterial = SAND;
  }
};

#endif //EXT_VANILLAGENERATOR_LIB_GENERATOR_GROUND_SANDYGROUNDGENERATOR_H_
