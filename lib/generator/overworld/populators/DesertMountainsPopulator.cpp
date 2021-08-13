#include <lib/objects/constants/BiomeList.h>
#include "DesertMountainsPopulator.h"

void DesertMountainsPopulator::InitPopulators() {
  waterLakeDecorator_.SetAmount(1);
}

std::vector<uint_fast8_t> DesertMountainsPopulator::GetBiomes() const {
  return {DESERT_MOUNTAINS};
}
