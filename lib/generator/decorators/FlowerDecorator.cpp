#include <lib/pocketmine/BlockList.h>
#include <lib/generator/objects/Flower.h>
#include "FlowerDecorator.h"

void FlowerDecorator::setFlowers(std::vector<FlowerDecoration> flowers) {
  decorations = std::move(flowers);
}

MinecraftBlock FlowerDecorator::getRandomFlower(Random random) {
  int totalWeight = 0;
  for (auto deco : decorations) totalWeight += deco.weight;

  int weight = static_cast<int>(random.nextBoundedInt(totalWeight));
  for (auto deco : decorations) {
    weight -= deco.weight;

    if (weight < 0) return deco.block;
  }

  return AIR;
}

void FlowerDecorator::decorate(SimpleChunkManager &world, Random &random, int chunkX, int chunkZ) {
  auto chunk = world.getChunk(chunkX, chunkZ);

  int x = static_cast<int>(random.nextBoundedInt(16));
  int z = static_cast<int>(random.nextBoundedInt(16));
  int source_y = static_cast<int>(random.nextBoundedInt(chunk->getHighestBlockAt(x, z) + 32));

  MinecraftBlock species = getRandomFlower(random);
  if (species.isObjectNull()) return;

  Flower(species).generate(world, random, (chunkX << 4) + x, source_y, (chunkZ << 4) + z);
}
