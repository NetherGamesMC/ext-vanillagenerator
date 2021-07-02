#include <set>
#include "OverworldPopulators.h"

OverworldPopulator::OverworldPopulator() {
  RegisterBiomePopulator(new BiomePopulator());
}

void OverworldPopulator::RegisterBiomePopulator(BiomePopulator *populator) {
  populator->InitPopulators();

  for (uint_fast8_t biome : populator->GetBiomes()) {
    biomePopulators.insert({biome, populator});
  }
}

void OverworldPopulator::Populate(SimpleChunkManager &world, Random &random, int_fast64_t chunkX, int_fast64_t chunkZ) {
  Chunk *chunk = world.getChunk(chunkX, chunkZ);
  uint_fast8_t biome = chunk->getBiomeArray()->get(8, 8);

  auto result = biomePopulators.find(biome);
  if (result != biomePopulators.end()) {
    result->second->Populate(world, random, chunkX, chunkZ);
  }
}

void OverworldPopulator::Destroy() {
  // Attempt to destroy multiple pointers of BiomePopulators
  std::set<BiomePopulator *> values;
  for (auto it : biomePopulators) {
    values.insert(it.second); // insert.second will be false if the value is already in the set
  }

  for (auto biomes : values) {
    biomes->Clean();

    delete biomes;
  }

  biomePopulators.clear();
}