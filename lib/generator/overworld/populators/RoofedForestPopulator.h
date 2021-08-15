#ifndef EXT_VANILLAGENERATOR_LIB_GENERATOR_OVERWORLD_POPULATORS_ROOFEDFORESTPOPULATOR_H_
#define EXT_VANILLAGENERATOR_LIB_GENERATOR_OVERWORLD_POPULATORS_ROOFEDFORESTPOPULATOR_H_

#include <lib/generator/object/trees/defaults/RedMushroomTree.h>
#include "ForestPopulator.h"

class RoofedForestPopulator : public ForestPopulator {
 public:
  void InitPopulators() override;

  [[nodiscard]] std::vector<uint_fast8_t> GetBiomes() const override;
 private:
  RedMushroomTree redMushroomTree;
  BrownMushroomTree brownMushroomTree;
  DarkOakTree darkOakTree;
};

#endif //EXT_VANILLAGENERATOR_LIB_GENERATOR_OVERWORLD_POPULATORS_ROOFEDFORESTPOPULATOR_H_
