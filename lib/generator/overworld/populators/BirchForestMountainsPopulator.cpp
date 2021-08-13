#include <lib/objects/constants/BiomeList.h>
#include "BirchForestMountainsPopulator.h"

void BirchForestMountainsPopulator::InitPopulators() {
  treeDecorator_.SetTrees({{1, birchTree}, {1, tallBirchTree}});
}

std::vector<uint_fast8_t> BirchForestMountainsPopulator::GetBiomes() const {
  return {BIRCH_FOREST_MOUNTAINS, BIRCH_FOREST_HILLS_MOUNTAINS};
}
