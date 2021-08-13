#include <lib/objects/constants/BiomeList.h>
#include "IcePlainsPopulator.h"

void IcePlainsPopulator::InitPopulators() {
  treeDecorator_.SetAmount(0);
  treeDecorator_.SetTrees({{1, redwoodTree}});
  flowerDecorator_.SetAmount(0);
}

std::vector<uint_fast8_t> IcePlainsPopulator::GetBiomes() const {
  return {ICE_PLAINS, ICE_MOUNTAINS};
}
