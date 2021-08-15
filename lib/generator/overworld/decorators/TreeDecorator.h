#ifndef EXT_VANILLAGENERATOR_LIB_GENERATOR_DECORATORS_TREEDECORATOR_H_
#define EXT_VANILLAGENERATOR_LIB_GENERATOR_DECORATORS_TREEDECORATOR_H_

#include <lib/generator/object/trees/GenericTree.h>
#include "Decorator.h"

class TreeDecorator : public Decorator {
 public:
  void SetTrees(std::vector<TreeDecoration> decorations);

  void Populate(ChunkManager &world, Random &random, int_fast32_t chunkX, int_fast32_t chunkZ) override;

  void Decorate(ChunkManager &world, Random &random, int_fast32_t chunkX, int_fast32_t chunkZ) override;
 private:
  /**
   * Returns a callback to Generate a GenericTree.
   */
  GenericTree *GetRandomTree(Random random);

  std::vector<TreeDecoration> decorations_;
};

#endif //EXT_VANILLAGENERATOR_LIB_GENERATOR_DECORATORS_TREEDECORATOR_H_
