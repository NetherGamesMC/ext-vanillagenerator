#include "BirchTree.h"

void BirchTree::Initialize(Random &random, BlockTransaction &txn) {
  GenericTree::Initialize(random, txn);

  SetHeight(random.NextInt(3) + 5);
  SetType(MAGIC_NUMBER_BIRCH);
}
