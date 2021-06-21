#include <lib/generator/objects/DoubleTallPlant.h>
#include <lib/pocketmine/BlockList.h>
#include "DoublePlantDecorator.h"

void DoublePlantDecorator::setDoublePlants(std::vector<DoublePlantDecoration> doublePlants) {
  decorations = std::move(doublePlants);
}

void DoublePlantDecorator::decorate(SimpleChunkManager &world, Random &random, int chunkX, int chunkZ) {
  auto chunk = world.getChunk(chunkX, chunkZ);

  int x = static_cast<int>(random.nextBoundedInt(16));
  int z = static_cast<int>(random.nextBoundedInt(16));
  int source_y = static_cast<int>(random.nextBoundedInt(chunk->getHighestBlockAt(x, z) + 32));

  MinecraftBlock species = getRandomDoublePlant(random);
  if (species.isObjectNull()) {
    return;
  }

  DoubleTallPlant(species).generate(world, random, (chunkX << 4) + x, source_y, (chunkZ << 4) + z);
}

MinecraftBlock DoublePlantDecorator::getRandomDoublePlant(Random random) {
  int totalWeight = 0;
  for (auto deco : decorations) {
    totalWeight += deco.weight;
  }

  int weight = static_cast<int>(random.nextBoundedInt(totalWeight));
  for (auto deco : decorations) {
    weight -= deco.weight;
    if (weight < 0) {
      return deco.block;
    }
  }

  return AIR;
}