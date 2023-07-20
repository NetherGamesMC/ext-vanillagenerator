#include "RedMushroomTree.h"

RedMushroomTree::RedMushroomTree() : BrownMushroomTree() {
  type = MCBlock::GetBlockFromStateId(BlockIds::RED_MUSHROOM_BLOCK);
}
