#include <lib/objects/constants/BlockList.h>
#include <lib/generator/object/Flower.h>
#include "FlowerDecorator.h"

void FlowerDecorator::SetFlowers(std::vector<FlowerDecoration> decorations) {
  decorations_ = std::move(decorations);
}

MinecraftBlock FlowerDecorator::GetRandomFlower(Random random) {
  int_fast32_t totalWeight = 0;
  for (auto deco : decorations_) totalWeight += deco.weight;

  int_fast32_t weight = static_cast<int>(random.NextInt(totalWeight));
  for (auto deco : decorations_) {
    weight -= deco.weight;

    if (weight < 0) return deco.block;
  }

  return AIR;
}

void FlowerDecorator::Decorate(ChunkManager &world, Random &random, int_fast32_t chunkX, int_fast32_t chunkZ) {
  auto chunk = world.GetChunk(chunkX, chunkZ);

  int_fast32_t x = random.NextInt(16);
  int_fast32_t z = random.NextInt(16);
  auto sourceY = static_cast<int_fast32_t>(random.NextInt(chunk->GetHighestBlockAt(x, z) + 32));

  MinecraftBlock species = GetRandomFlower(random);
  if (species.IsObjectNull()) return;

  Flower(species).Generate(world, random, (chunkX << 4) + x, sourceY, (chunkZ << 4) + z);
}
