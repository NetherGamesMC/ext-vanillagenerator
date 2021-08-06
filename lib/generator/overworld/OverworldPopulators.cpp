#include <memory>
#include <set>
#include "OverworldPopulators.h"

OverworldPopulator::OverworldPopulator() {
  RegisterBiomePopulator(defaultPopulator);
}

void OverworldPopulator::RegisterBiomePopulator(BiomePopulator &populator) {
  populator.InitPopulators();

  for (uint_fast8_t biome : populator.GetBiomes()) {
    biomePopulators.insert({biome, populator});
  }
}

void OverworldPopulator::Populate(ChunkManager &world, Random &random, int_fast32_t chunkX, int_fast32_t chunkZ) {
  Chunk *chunk = world.GetChunk(chunkX, chunkZ);
  uint_fast8_t biome = chunk->GetBiomeArray()->Get(8, 8);

  auto result = biomePopulators.find(biome);
  if (result != biomePopulators.end()) {
    result->second.Populate(world, random, chunkX, chunkZ);
  }
}

OverworldPopulator::~OverworldPopulator() {
  biomePopulators.clear();
}
