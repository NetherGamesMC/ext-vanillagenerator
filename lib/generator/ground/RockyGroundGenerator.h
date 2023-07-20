#ifndef EXT_VANILLAGENERATOR_LIB_GENERATOR_GROUND_ROCKYGROUNDGENERATOR_H_
#define EXT_VANILLAGENERATOR_LIB_GENERATOR_GROUND_ROCKYGROUNDGENERATOR_H_

#include "GroundGenerator.h"

class RockyGroundGenerator : public GroundGenerator {
 public:
  RockyGroundGenerator() {
    topMaterial = MCBlock::GetBlockFromStateId(BlockIds::STONE);
    groundMaterial = MCBlock::GetBlockFromStateId(BlockIds::STONE);
  }
};

#endif //EXT_VANILLAGENERATOR_LIB_GENERATOR_GROUND_ROCKYGROUNDGENERATOR_H_
