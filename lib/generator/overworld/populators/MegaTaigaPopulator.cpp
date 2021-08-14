#include <lib/objects/constants/BiomeList.h>
#include "MegaTaigaPopulator.h"

void MegaTaigaPopulator::InitPopulators() {
  treeDecorator_.SetAmount(10);
  treeDecorator_.SetTrees({{52, redwoodTree}, {26, tallRedwoodTree}, {36, megaPineTree}, {3, megaSpruceTree}});
  deadBushDecorator_.SetAmount(0);
  taigaBrownMushroomDecorator.SetAmount(3);
  taigaBrownMushroomDecorator.SetUseFixedHeightRange();
  taigaBrownMushroomDecorator.SetDensity(0.25);
  taigaRedMushroomDecorator.SetAmount(3);
  taigaRedMushroomDecorator.SetDensity(0.125);
}

std::vector<uint_fast8_t> MegaTaigaPopulator::GetBiomes() const {
  return {MEGA_TAIGA, MEGA_TAIGA_HILLS};
}

// TODO: StoneBoulderDecorator, for now, it is not really an important decorator.