#ifndef EXT_VANILLAGENERATOR_LIB_GENERATOR_GROUND_MYCELGROUNDGENERATOR_H_
#define EXT_VANILLAGENERATOR_LIB_GENERATOR_GROUND_MYCELGROUNDGENERATOR_H_

#include "GroundGenerator.h"

class MycelGroundGenerator : public GroundGenerator {
 public:
  MycelGroundGenerator() {
    topMaterial = MCBlock::GetBlockFromStateId(BlockIds::MYCELIUM);
  }
};

#endif //EXT_VANILLAGENERATOR_LIB_GENERATOR_GROUND_MYCELGROUNDGENERATOR_H_
