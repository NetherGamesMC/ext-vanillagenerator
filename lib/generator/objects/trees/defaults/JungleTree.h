#ifndef EXT_NOISELIB_LIB_GENERATOR_OBJECTS_TREES_DEFAULTS_JUNGLETREE_H_
#define EXT_NOISELIB_LIB_GENERATOR_OBJECTS_TREES_DEFAULTS_JUNGLETREE_H_

#include "../GenericTree.h"

class JungleTree : public GenericTree {
 public:
  void Initialize(Random &random, BlockTransaction &txn) override;
};

#endif //EXT_NOISELIB_LIB_GENERATOR_OBJECTS_TREES_DEFAULTS_JUNGLETREE_H_
