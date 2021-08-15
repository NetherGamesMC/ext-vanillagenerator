#ifndef EXT_VANILLAGENERATOR_LIB_GENERATOR_POPULATORS_ICEPLAINSSPIKESPOPULATOR_H_
#define EXT_VANILLAGENERATOR_LIB_GENERATOR_POPULATORS_ICEPLAINSSPIKESPOPULATOR_H_

#include "IcePlainsPopulator.h"

class IcePlainsSpikesPopulator : public IcePlainsPopulator {
 public:
  void InitPopulators() override;

  [[nodiscard]] std::vector<uint_fast8_t> GetBiomes() const override;
};

#endif //EXT_VANILLAGENERATOR_LIB_GENERATOR_POPULATORS_ICEPLAINSSPIKESPOPULATOR_H_
