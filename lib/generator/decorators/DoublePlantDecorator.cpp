#include <lib/generator/objects/DoubleTallPlant.h>
#include <lib/pocketmine/BlockList.h>
#include "DoublePlantDecorator.h"

void DoublePlantDecorator::SetDoublePlants(std::vector<DoublePlantDecoration> doublePlants) {
  decorations_ = std::move(doublePlants);
}

void DoublePlantDecorator::Decorate(SimpleChunkManager &world, Random &random, int_fast64_t chunkX, int_fast64_t chunkZ) {
  auto chunk = world.getChunk(chunkX, chunkZ);

  int_fast64_t x = random.nextInt(16);
  int_fast64_t z = random.nextInt(16);
  auto source_y = static_cast<int_fast32_t>(random.nextInt(chunk->getHighestBlockAt(x, z) + 32));

  MinecraftBlock species = GetRandomDoublePlant(random);
  if (species.isObjectNull()) {
    return;
  }

  DoubleTallPlant(species).Generate(world, random, (chunkX << 4) + x, source_y, (chunkZ << 4) + z);
}

MinecraftBlock DoublePlantDecorator::GetRandomDoublePlant(Random random) {
  int totalWeight = 0;
  for (auto deco : decorations_)totalWeight += deco.weight;

  int weight = static_cast<int>(random.nextInt(totalWeight));
  for (auto deco : decorations_) {
    weight -= deco.weight;

    if (weight < 0)return deco.block;
  }

  return AIR;
}