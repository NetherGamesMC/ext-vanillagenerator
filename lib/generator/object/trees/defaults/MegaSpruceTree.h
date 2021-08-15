#ifndef EXT_NOISELIB_LIB_GENERATOR_OBJECTS_TREES_DEFAULTS_MEGASPRUCETREE_H_
#define EXT_NOISELIB_LIB_GENERATOR_OBJECTS_TREES_DEFAULTS_MEGASPRUCETREE_H_

#include "MegaPineTree.h"

class MegaSpruceTree : public MegaPineTree {
 public:
  void Initialize(Random &random, BlockTransaction &txn) override;
};

#endif //EXT_NOISELIB_LIB_GENERATOR_OBJECTS_TREES_DEFAULTS_MEGASPRUCETREE_H_