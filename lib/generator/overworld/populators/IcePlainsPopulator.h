#ifndef EXT_VANILLAGENERATOR_LIB_GENERATOR_POPULATORS_ICEPLAINSPOPULATOR_H_
#define EXT_VANILLAGENERATOR_LIB_GENERATOR_POPULATORS_ICEPLAINSPOPULATOR_H_

#include <lib/generator/object/trees/defaults/RedwoodTree.h>
#include "BiomePopulator.h"

class IcePlainsPopulator : public BiomePopulator {
 public:
  void InitPopulators() override;

  [[nodiscard]] std::vector<uint_fast8_t> GetBiomes() const override;
 protected:
  RedwoodTree redwoodTree;
};

#endif //EXT_VANILLAGENERATOR_LIB_GENERATOR_POPULATORS_ICEPLAINSPOPULATOR_H_
