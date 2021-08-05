#include <memory>
#include <set>
#include "OverworldPopulators.h"

OverworldPopulator::OverworldPopulator() {
  RegisterBiomePopulator(std::make_unique<BiomePopulator>());
}

void OverworldPopulator::RegisterBiomePopulator(std::unique_ptr<BiomePopulator> populator) {
  populator->InitPopulators();

  for (uint_fast8_t biome : populator->GetBiomes()) {
    biomePopulators.insert({biome, std::move(populator)});
  }
}

void OverworldPopulator::Populate(ChunkManager &world, Random &random, int_fast64_t chunkX, int_fast64_t chunkZ) {
  Chunk *chunk = world.GetChunk(chunkX, chunkZ);
  uint_fast8_t biome = chunk->GetBiomeArray()->Get(8, 8);

  auto result = biomePopulators.find(biome);
  if (result != biomePopulators.end()) {
    result->second->Populate(world, random, chunkX, chunkZ);
  }
}

OverworldPopulator::~OverworldPopulator() {
  biomePopulators.clear();
}
