#ifndef EXT_VANILLAGENERATOR_LIB_GENERATOR_OVERWORLD_POPULATORS_SAVANNAPOPULATOR_H_
#define EXT_VANILLAGENERATOR_LIB_GENERATOR_OVERWORLD_POPULATORS_SAVANNAPOPULATOR_H_

#include "BiomePopulator.h"

class SavannaPopulator : public BiomePopulator {
 public:
  void InitPopulators() override;

  [[nodiscard]] std::vector<uint_fast8_t> GetBiomes() const override;

 protected:
  AcaciaTree acaciaTree;
  GenericTree genericTree;
};

#endif //EXT_VANILLAGENERATOR_LIB_GENERATOR_OVERWORLD_POPULATORS_SAVANNAPOPULATOR_H_
