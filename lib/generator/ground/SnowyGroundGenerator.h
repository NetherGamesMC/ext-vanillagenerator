#ifndef EXT_VANILLAGENERATOR_LIB_GENERATOR_GROUND_SNOWYGROUNDGENERATOR_H_
#define EXT_VANILLAGENERATOR_LIB_GENERATOR_GROUND_SNOWYGROUNDGENERATOR_H_

#include "GroundGenerator.h"

class SnowyGroundGenerator : public GroundGenerator {
 public:
  SnowyGroundGenerator() {
    topMaterial = MCBlock::GetBlockFromStateId(BlockIds::SNOW);
  }
};

#endif //EXT_VANILLAGENERATOR_LIB_GENERATOR_GROUND_SNOWYGROUNDGENERATOR_H_
