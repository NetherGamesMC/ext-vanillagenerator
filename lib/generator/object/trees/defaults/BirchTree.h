#ifndef EXT_VANILLAGENERATOR_LIB_GENERATOR_OBJECTS_TREES_DEFAULTS_BIRCHTREE_H_
#define EXT_VANILLAGENERATOR_LIB_GENERATOR_OBJECTS_TREES_DEFAULTS_BIRCHTREE_H_

#include <lib/generator/object/trees/GenericTree.h>

class BirchTree : public GenericTree {
 public:
  void Initialize(Random &random, BlockTransaction &txn) override;
};

#endif //EXT_VANILLAGENERATOR_LIB_GENERATOR_OBJECTS_TREES_DEFAULTS_BIRCHTREE_H_
