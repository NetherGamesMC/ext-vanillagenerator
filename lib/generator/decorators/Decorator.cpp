#include "Decorator.h"

void Decorator::setAmount(int amountVal) {
  amount = amountVal;
}

void Decorator::populate(SimpleChunkManager &world, Random &random, int chunkX, int chunkZ) {
  for (int i = 0; i < amount; ++i) {
    decorate(world, random, chunkX, chunkZ);
  }
}