#include <lib/objects/constants/BiomeList.h>
#include "MegaSpruceTaigaPopulator.h"

void MegaSpruceTaigaPopulator::InitPopulators() {
  MegaTaigaPopulator::InitPopulators();

  treeDecorator_.SetTrees({{44, redwoodTree}, {22, tallRedwoodTree}, {33, megaSpruceTree}});
}

std::vector<uint_fast8_t> MegaSpruceTaigaPopulator::GetBiomes() const {
  return {MEGA_SPRUCE_TAIGA, MEGA_SPRUCE_TAIGA_HILLS};
}
