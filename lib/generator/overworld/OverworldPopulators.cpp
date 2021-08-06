#include <memory>
#include <set>
#include "OverworldPopulators.h"

OverworldPopulator::OverworldPopulator() {
}

void OverworldPopulator::RegisterBiomePopulator(BiomePopulator *populator) {
  populator->InitPopulators();

  for (uint_fast8_t biome : populator->GetBiomes()) {
    biomePopulators.insert({biome, populator});
  }
}

void OverworldPopulator::Populate(ChunkManager &world, Random &random, int_fast32_t chunkX, int_fast32_t chunkZ) {
  BiomePopulator().Populate(world, random, chunkX, chunkZ);
}

OverworldPopulator::~OverworldPopulator() {
  for (auto x : biomePopulators) {
    delete x.second;
  }

  biomePopulators.clear();
}
