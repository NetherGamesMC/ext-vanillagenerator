#ifndef EXT_VANILLAGENERATOR_LIB_GENERATOR_POPULATORS_BIRCHFORESTPOPULATOR_H_
#define EXT_VANILLAGENERATOR_LIB_GENERATOR_POPULATORS_BIRCHFORESTPOPULATOR_H_

#include "ForestPopulator.h"

class BirchForestPopulator : public ForestPopulator {
 public:
  void InitPopulators() override;

  [[nodiscard]] std::vector<uint_fast8_t> GetBiomes() const override;
};

#endif //EXT_VANILLAGENERATOR_LIB_GENERATOR_POPULATORS_BIRCHFORESTPOPULATOR_H_
