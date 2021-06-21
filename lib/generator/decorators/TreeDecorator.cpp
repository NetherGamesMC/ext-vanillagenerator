#include "TreeDecorator.h"

void TreeDecorator::setTrees(std::vector<TreeDecoration> trees) {
  decorations = std::move(trees);
}

TreeObject TreeDecorator::getRandomTree(Random random) {
  int totalWeight = 0;
  for (auto deco : decorations) {
    totalWeight += deco.weight;
  }

  int weight = static_cast<int>(random.nextBoundedInt(totalWeight));
  for (auto deco : decorations) {
    weight -= deco.weight;
    if (weight < 0) {
      return deco.callback;
    }
  }

  return nullptr;
}

void TreeDecorator::decorate(SimpleChunkManager &world, Random &random, int chunkX, int chunkZ) {
  auto chunk = world.getChunk(chunkX, chunkZ);

  int x = random.nextBoundedInt(16);
  int z = random.nextBoundedInt(16);
  int source_y = chunk->getHighestBlockAt(x, z);

  TreeObject treeObject = getRandomTree(random);
  if (treeObject != nullptr) {
    BlockTransaction txn = BlockTransaction(world);
    auto tree = treeObject(random, txn);

    if (tree.generate(world, random, (chunkX << 4) + x, source_y, (chunkZ << 4) + z)) {
      txn.applyBlockChanges();
    }

    txn.destroy();
  }
}

void TreeDecorator::populate(SimpleChunkManager &world, Random &random, int chunkX, int chunkZ) {
  int treeAmount = amount;
  if (random.nextBoundedInt(10) == 0) {
    ++treeAmount;
  }

  for (int i = 0; i < treeAmount; ++i) {
    decorate(world, random, chunkX, chunkZ);
  }
}
