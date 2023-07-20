#include <lib/generator/object/DoubleTallPlant.h>
#include "DoublePlantDecorator.h"

void DoublePlantDecorator::SetDoublePlants(std::vector<DoublePlantDecoration> doublePlants) {
  decorations_ = std::move(doublePlants);
}

void DoublePlantDecorator::Decorate(ChunkManager &world, Random &random, int_fast32_t chunkX, int_fast32_t chunkZ) {
  auto chunk = world.GetChunk(chunkX, chunkZ);

  int_fast32_t x = random.NextInt(16);
  int_fast32_t z = random.NextInt(16);
  auto sourceY = static_cast<int_fast32_t>(random.NextInt(chunk->GetHighestBlockAt(x, z) + 32));

  const MCBlock *species = GetRandomDoublePlant(random);
  if (species == nullptr) {
    return;
  }

  DoubleTallPlant(species).Generate(world, random, (chunkX << 4) + x, sourceY, (chunkZ << 4) + z);
}

const MCBlock *DoublePlantDecorator::GetRandomDoublePlant(Random random) {
  int_fast32_t totalWeight = 0;
  for (auto deco : decorations_)totalWeight += deco.weight;

  int_fast32_t weight = static_cast<int>(random.NextInt(totalWeight));
  for (auto deco : decorations_) {
    weight -= deco.weight;

    if (weight < 0)return deco.block;
  }

  return nullptr;
}