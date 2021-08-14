#include <lib/objects/constants/BiomeList.h>
#include "BirchForestPopulator.h"

void BirchForestPopulator::InitPopulators() {
  treeDecorator_.SetAmount(10);
  treeDecorator_.SetTrees({{1, birchTree}});
}

std::vector<uint_fast8_t> BirchForestPopulator::GetBiomes() const {
  return {BIRCH_FOREST, BIRCH_FOREST_HILLS};
}
