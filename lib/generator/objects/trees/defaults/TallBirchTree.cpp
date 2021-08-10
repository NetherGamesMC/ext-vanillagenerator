#include "TallBirchTree.h"

void TallBirchTree::Initialize(Random &random, BlockTransaction &txn) {
  BirchTree::Initialize(random, txn);

  SetHeight(height + static_cast<int_fast32_t>(random.NextInt(7)));
}
