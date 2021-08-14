#include <lib/objects/constants/BiomeList.h>
#include "RoofedForestPopulator.h"

void RoofedForestPopulator::InitPopulators() {
  treeDecorator_.SetAmount(50);
  treeDecorator_.SetTrees({{20, genericTree}, {5, birchTree}, {1, redMushroomTree}, {1, brownMushroomTree}, {20, darkOakTree}});

  tallGrassDecorator_.SetAmount(0);
}

std::vector<uint_fast8_t> RoofedForestPopulator::GetBiomes() const {
  return {ROOFED_FOREST, ROOFED_FOREST_MOUNTAINS};
}
