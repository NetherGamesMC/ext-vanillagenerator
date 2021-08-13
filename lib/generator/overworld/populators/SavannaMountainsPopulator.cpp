#include <lib/objects/constants/BiomeList.h>
#include "SavannaMountainsPopulator.h"

void SavannaMountainsPopulator::InitPopulators() {
  treeDecorator_.SetAmount(2);
  treeDecorator_.SetTrees({{4, acaciaTree}, {4, genericTree}});
  flowerDecorator_.SetAmount(2);
  tallGrassDecorator_.SetAmount(5);

  doublePlantDecorator_.SetDoublePlants({{1, DOUBLE_TALLGRASS}});
}

std::vector<uint_fast8_t> SavannaMountainsPopulator::GetBiomes() const {
  return {SAVANNA_MOUNTAINS, SAVANNA_PLATEAU_MOUNTAINS};
}


