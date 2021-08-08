#include "NetherPopulator.h"

NetherPopulator::NetherPopulator() {
  RegisterBiomePopulator(defaultPopulator);
}

void NetherPopulator::RegisterBiomePopulator(NetherBiomePopulator &populator) {
  populator.InitPopulators();
}

void NetherPopulator::Populate(ChunkManager &world, Random &random, int_fast32_t chunkX, int_fast32_t chunkZ) {
  defaultPopulator.Populate(world, random, chunkX, chunkZ);
}