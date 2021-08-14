#include "TreeDecorator.h"

void TreeDecorator::SetTrees(std::vector<TreeDecoration> decorations) {
  decorations_ = std::move(decorations);
}

GenericTree *TreeDecorator::GetRandomTree(Random random) {
  int_fast32_t totalWeight = 0;
  for (auto deco : decorations_) {
    totalWeight += deco.weight;
  }

  auto weight = static_cast<int_fast32_t>(random.NextInt(totalWeight));
  for (auto deco : decorations_) {
    weight -= deco.weight;
    if (weight < 0) {
      return &deco.treeObject;
    }
  }

  return nullptr;
}

void TreeDecorator::Decorate(ChunkManager &world, Random &random, int_fast32_t chunkX, int_fast32_t chunkZ) {
  auto chunk = world.GetChunk(chunkX, chunkZ);

  int_fast32_t x = random.NextInt(16);
  int_fast32_t z = random.NextInt(16);
  int_fast32_t sourceY = chunk->GetHighestBlockAt(x, z);

  GenericTree *tree = GetRandomTree(random);
  if (tree != nullptr) {
    BlockTransaction txn = BlockTransaction(world);
    tree->Initialize(random, txn);

    if (tree->Generate(world, random, (chunkX << 4) + x, sourceY, (chunkZ << 4) + z)) {
      txn.ApplyBlockChanges();
    }
  }
}

void TreeDecorator::Populate(ChunkManager &world, Random &random, int_fast32_t chunkX, int_fast32_t chunkZ) {
  int_fast32_t treeAmount = amount_;
  if (random.NextInt(10) == 0) {
    ++treeAmount;
  }

  for (int_fast32_t i = 0; i < treeAmount; ++i) {
    Decorate(world, random, chunkX, chunkZ);
  }
}
