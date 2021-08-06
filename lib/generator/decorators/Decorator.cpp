#include "Decorator.h"

void Decorator::SetAmount(int amount) {
  amount_ = amount;
}

void Decorator::Populate(ChunkManager &world, Random &random, int_fast32_t chunkX, int_fast32_t chunkZ) {
  for (int i = 0; i < amount_; ++i) {
    Decorate(world, random, chunkX, chunkZ);
  }
}