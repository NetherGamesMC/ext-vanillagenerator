#ifndef EXT_VANILLAGENERATOR_LIB_GENERATOR_OVERWORLD_POPULATORS_MEGASPRUCETAIGAPOPULATOR_H_
#define EXT_VANILLAGENERATOR_LIB_GENERATOR_OVERWORLD_POPULATORS_MEGASPRUCETAIGAPOPULATOR_H_

#include "MegaTaigaPopulator.h"

class MegaSpruceTaigaPopulator : public MegaTaigaPopulator {
 public:
  void InitPopulators() override;

  [[nodiscard]] std::vector<uint_fast8_t> GetBiomes() const override;
};

#endif //EXT_VANILLAGENERATOR_LIB_GENERATOR_OVERWORLD_POPULATORS_MEGASPRUCETAIGAPOPULATOR_H_
