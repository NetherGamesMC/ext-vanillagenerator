#include "NetherBiomePopulator.h"

NetherBiomePopulator::NetherBiomePopulator() = default;

void NetherBiomePopulator::InitPopulators() {
  fireDecorator_.SetAmount(1);
  glowstoneDecorator_.SetAmount(1);
  glowstoneDecorator2_.SetAmount(1);
  brownMushroomDecorator_.SetAmount(1);
  redMushroomDecorator_.SetAmount(1);
}

void NetherBiomePopulator::Populate(ChunkManager &world, Random &random, int_fast32_t chunkX, int_fast32_t chunkZ) {
  fireDecorator_.Populate(world, random, chunkX, chunkZ);
  glowstoneDecorator_.Populate(world, random, chunkX, chunkZ);
  glowstoneDecorator2_.Populate(world, random, chunkX, chunkZ);
  brownMushroomDecorator_.Populate(world, random, chunkX, chunkZ);
  redMushroomDecorator_.Populate(world, random, chunkX, chunkZ);
}