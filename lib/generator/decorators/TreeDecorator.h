#ifndef EXT_NOISELIB_LIB_GENERATOR_DECORATORS_TREEDECORATOR_H_
#define EXT_NOISELIB_LIB_GENERATOR_DECORATORS_TREEDECORATOR_H_

#include <lib/generator/objects/GenericTree.h>
#include "Decorator.h"

class TreeDecorator : public Decorator {
 public:
  void setTrees(std::vector<TreeDecoration> trees);

  void populate(SimpleChunkManager &world, Random &random, int chunkX, int chunkZ) override;

  void decorate(SimpleChunkManager &world, Random &random, int chunkX, int chunkZ) override;
 private:
  /**
   * Returns a callback to generate a GenericTree.
   */
  TreeObject getRandomTree(Random random);

  std::vector<TreeDecoration> decorations;
};

#endif //EXT_NOISELIB_LIB_GENERATOR_DECORATORS_TREEDECORATOR_H_
