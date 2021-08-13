#ifndef EXT_NOISELIB_LIB_GENERATOR_OBJECTS_TREES_DEFAULTS_TALLBIRCHTREE_H_
#define EXT_NOISELIB_LIB_GENERATOR_OBJECTS_TREES_DEFAULTS_TALLBIRCHTREE_H_

#include "BirchTree.h"

class TallBirchTree : public BirchTree {
 public:
  void Initialize(Random &random, BlockTransaction &txn) override;
};

#endif //EXT_NOISELIB_LIB_GENERATOR_OBJECTS_TREES_DEFAULTS_TALLBIRCHTREE_H_
