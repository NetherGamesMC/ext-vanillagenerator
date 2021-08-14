#include <lib/objects/constants/BiomeList.h>
#include "JungleEdgePopulator.h"

void JungleEdgePopulator::InitPopulators() {
  treeDecorator_.SetAmount(2);
  treeDecorator_.SetTrees({{10, bigOakTree}, {45, cocoaTree}});
}

std::vector<uint_fast8_t> JungleEdgePopulator::GetBiomes() const {
  return {JUNGLE_EDGE, JUNGLE_EDGE_MOUNTAINS};
}

