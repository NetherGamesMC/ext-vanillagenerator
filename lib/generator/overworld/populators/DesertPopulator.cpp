#include <lib/objects/constants/BiomeList.h>
#include "DesertPopulator.h"

void DesertPopulator::InitPopulators() {
  waterLakeDecorator_.SetAmount(0);
  deadBushDecorator_.SetAmount(2);
  sugarCaneDecorator_.SetAmount(60);
  cactusDecorator_.SetAmount(3);
}

std::vector<uint_fast8_t> DesertPopulator::GetBiomes() const {
  return {DESERT, DESERT_HILLS};
}
