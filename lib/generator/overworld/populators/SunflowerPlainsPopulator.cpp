#include <lib/objects/constants/BiomeList.h>
#include "SunflowerPlainsPopulator.h"

void SunflowerPlainsPopulator::InitPopulators() {
  PlainsPopulator::InitPopulators();

  doublePlantDecorator_.SetAmount(10);
  doublePlantDecorator_.SetDoublePlants({{1, MCBlock::GetBlockFromStateId(BlockIds::SUNFLOWER)}});
}

std::vector<uint_fast8_t> SunflowerPlainsPopulator::GetBiomes() const {
  return {SUNFLOWER_PLAINS};
}
