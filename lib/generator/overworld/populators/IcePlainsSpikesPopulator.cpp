#include <lib/objects/constants/BiomeList.h>
#include "IcePlainsSpikesPopulator.h"

void IcePlainsSpikesPopulator::InitPopulators() {
  IcePlainsPopulator::InitPopulators();

  tallGrassDecorator_.SetAmount(0);
  // TODO: Ice decorator.
}

std::vector<uint_fast8_t> IcePlainsSpikesPopulator::GetBiomes() const {
  return {ICE_PLAINS_SPIKES};
}
