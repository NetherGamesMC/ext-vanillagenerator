#include <set>
#include "OverworldPopulators.h"

OverworldPopulator::OverworldPopulator() {
  registerBiomePopulator(new BiomePopulator());
}

void OverworldPopulator::registerBiomePopulator(BiomePopulator *populator) {
  populator->initPopulators();

  for (uint_fast8_t biome : populator->getBiomes()) {
    biomePopulators.insert({biome, populator});
  }
}

void OverworldPopulator::populate(SimpleChunkManager world, Random random, int chunkX, int chunkZ) {
  Chunk *chunk = world.getChunk(chunkX, chunkZ);
  uint_fast8_t biome = chunk->getBiomeArray()->get(8, 8);

  auto result = biomePopulators.find(biome);
  if (result != biomePopulators.end()) {
    result->second->populate(world, random, chunkX, chunkZ);
  }
}

void OverworldPopulator::destroy() {
  // Attempt to destroy multiple pointers of BiomePopulators
  std::set<BiomePopulator *> values;
  for (auto it : biomePopulators) {
    values.insert(it.second); // insert.second will be false if the value is already in the set
  }

  for (auto biomes : values) {
    biomes->clean();

    delete biomes;
  }

  biomePopulators.clear();
}