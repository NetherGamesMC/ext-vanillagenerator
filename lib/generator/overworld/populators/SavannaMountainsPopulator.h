#ifndef EXT_VANILLAGENERATOR_LIB_GENERATOR_OVERWORLD_POPULATORS_SAVANNAMOUNTAINSPOPULATOR_H_
#define EXT_VANILLAGENERATOR_LIB_GENERATOR_OVERWORLD_POPULATORS_SAVANNAMOUNTAINSPOPULATOR_H_

#include "SavannaPopulator.h"

class SavannaMountainsPopulator : public SavannaPopulator {
 public:
  void InitPopulators() override;

  [[nodiscard]] std::vector<uint_fast8_t> GetBiomes() const override;
};

#endif //EXT_VANILLAGENERATOR_LIB_GENERATOR_OVERWORLD_POPULATORS_SAVANNAMOUNTAINSPOPULATOR_H_
