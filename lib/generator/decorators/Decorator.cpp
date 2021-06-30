#include "Decorator.h"

void Decorator::SetAmount(int amount) {
  amount_ = amount;
}

void Decorator::Populate(SimpleChunkManager &world, Random &random, int_fast64_t chunkX, int_fast64_t chunkZ) {
  for (int i = 0; i < amount_; ++i) {
    Decorate(world, random, chunkX, chunkZ);
  }
}