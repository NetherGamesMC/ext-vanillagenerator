#ifndef EXT_VANILLAGENERATOR_LIB_GENERATOR_GROUND_SANDYGROUNDGENERATOR_H_
#define EXT_VANILLAGENERATOR_LIB_GENERATOR_GROUND_SANDYGROUNDGENERATOR_H_

#include "GroundGenerator.h"

class SandyGroundGenerator : public GroundGenerator {
 public:
  SandyGroundGenerator() {
    topMaterial = MCBlock::GetBlockFromStateId(BlockIds::SAND);
    groundMaterial = MCBlock::GetBlockFromStateId(BlockIds::SAND);
  }
};

#endif //EXT_VANILLAGENERATOR_LIB_GENERATOR_GROUND_SANDYGROUNDGENERATOR_H_
