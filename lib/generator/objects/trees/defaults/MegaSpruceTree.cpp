#include "MegaSpruceTree.h"

void MegaSpruceTree::Initialize(Random &random, BlockTransaction &txn) {
  MegaPineTree::Initialize(random, txn);

  SetLeavesHeight(leavesHeight + 10);
}
