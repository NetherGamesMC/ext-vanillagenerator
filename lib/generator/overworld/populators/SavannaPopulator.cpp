#include <lib/objects/constants/BiomeList.h>
#include "SavannaPopulator.h"

void SavannaPopulator::InitPopulators() {
  doublePlantDecorator_.SetAmount(7);
  doublePlantDecorator_.SetDoublePlants({{1, DOUBLE_TALLGRASS}});
  treeDecorator_.SetAmount(1);
  treeDecorator_.SetTrees({{4, acaciaTree}, {4, genericTree}});
  flowerDecorator_.SetAmount(4);
  tallGrassDecorator_.SetAmount(20);
}

std::vector<uint_fast8_t> SavannaPopulator::GetBiomes() const {
  return {SAVANNA, SAVANNA_PLATEAU};
}

