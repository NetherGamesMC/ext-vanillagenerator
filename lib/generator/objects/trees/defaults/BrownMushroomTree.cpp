#include "BrownMushroomTree.h"

void BrownMushroomTree::Initialize(Random &random, BlockTransaction &txn) {
  GenericTree::Initialize(random, txn);

  SetOverrides({0, 18, 161});
  SetHeight(static_cast<int_fast32_t>(random.NextInt(3)) + 4);
}

bool BrownMushroomTree::CanPlaceOn(MinecraftBlock soil) {
  return soil == GRASS || soil == DIRT || soil == MYCELIUM;
}



