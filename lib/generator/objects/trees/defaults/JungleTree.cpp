#include "JungleTree.h"

void JungleTree::Initialize(Random &random, BlockTransaction &txn) {
  GenericTree::Initialize(random, txn);

  SetHeight(static_cast<int_fast32_t>(random.NextInt(7)) + 4);
  SetType(MAGIC_NUMBER_JUNGLE);
}