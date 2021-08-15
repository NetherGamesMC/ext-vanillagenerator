#ifndef EXT_VANILLAGENERATOR_LIB_GENERATOR_POPULATORS_BIRCHFORESTMOUNTAINSPOPULATOR_H_
#define EXT_VANILLAGENERATOR_LIB_GENERATOR_POPULATORS_BIRCHFORESTMOUNTAINSPOPULATOR_H_

#include <lib/generator/object/trees/defaults/TallBirchTree.h>
#include "ForestPopulator.h"

class BirchForestMountainsPopulator : public ForestPopulator {
 public:
  void InitPopulators() override;

  [[nodiscard]] std::vector<uint_fast8_t> GetBiomes() const override;
 protected:
  TallBirchTree tallBirchTree;
};

#endif //EXT_VANILLAGENERATOR_LIB_GENERATOR_POPULATORS_BIRCHFORESTMOUNTAINSPOPULATOR_H_
