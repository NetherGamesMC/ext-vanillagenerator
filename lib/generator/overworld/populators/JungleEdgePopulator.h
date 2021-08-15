#ifndef EXT_VANILLAGENERATOR_LIB_GENERATOR_OVERWORLD_POPULATORS_JUNGLEEDGEPOPULATOR_H_
#define EXT_VANILLAGENERATOR_LIB_GENERATOR_OVERWORLD_POPULATORS_JUNGLEEDGEPOPULATOR_H_

#include "JunglePopulator.h"

class JungleEdgePopulator : public JunglePopulator {
 public:
  void InitPopulators() override;

  [[nodiscard]] std::vector<uint_fast8_t> GetBiomes() const override;
};

#endif //EXT_VANILLAGENERATOR_LIB_GENERATOR_OVERWORLD_POPULATORS_JUNGLEEDGEPOPULATOR_H_
