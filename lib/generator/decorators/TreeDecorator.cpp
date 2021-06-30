#include "TreeDecorator.h"

void TreeDecorator::setTrees(std::vector<TreeDecoration> decorations) {
  decorations_ = std::move(decorations);
}

TreeObject TreeDecorator::GetRandomTree(Random random) {
  int_fast32_t totalWeight = 0;
  for (auto deco : decorations_) {
    totalWeight += deco.weight;
  }

  auto weight = static_cast<int_fast32_t>(random.nextInt(totalWeight));
  for (auto deco : decorations_) {
    weight -= deco.weight;
    if (weight < 0) {
      return deco.callback;
    }
  }

  return nullptr;
}

void TreeDecorator::Decorate(SimpleChunkManager &world, Random &random, int_fast64_t chunkX, int_fast64_t chunkZ) {
  auto chunk = world.getChunk(chunkX, chunkZ);

  int_fast64_t x = random.nextInt(16);
  int_fast64_t z = random.nextInt(16);
  int_fast32_t source_y = chunk->getHighestBlockAt(x, z);

  TreeObject treeObject = GetRandomTree(random);
  if (treeObject != nullptr) {
    BlockTransaction txn = BlockTransaction(world);
    auto tree = treeObject(random, txn);

    if (tree.Generate(world, random, (chunkX << 4) + x, source_y, (chunkZ << 4) + z)) {
      txn.ApplyBlockChanges();
    }
  }
}

void TreeDecorator::Populate(SimpleChunkManager &world, Random &random, int_fast64_t chunkX, int_fast64_t chunkZ) {
  int treeAmount = amount_;
  if (random.nextInt(10) == 0) {
    ++treeAmount;
  }

  for (int i = 0; i < treeAmount; ++i) {
    Decorate(world, random, chunkX, chunkZ);
  }
}
