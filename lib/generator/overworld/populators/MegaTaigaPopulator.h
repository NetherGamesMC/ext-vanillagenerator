#ifndef EXT_VANILLAGENERATOR_LIB_GENERATOR_OVERWORLD_POPULATORS_MEGATAIGAPOPULATOR_H_
#define EXT_VANILLAGENERATOR_LIB_GENERATOR_OVERWORLD_POPULATORS_MEGATAIGAPOPULATOR_H_

#include <lib/generator/object/trees/defaults/MegaPineTree.h>
#include <lib/generator/object/trees/defaults/MegaSpruceTree.h>
#include "lib/generator/overworld/populators/TaigaPopulator.h"

class MegaTaigaPopulator : public TaigaPopulator {
 public:
  void InitPopulators() override;

  [[nodiscard]] std::vector<uint_fast8_t> GetBiomes() const override;
 protected:
  MegaPineTree megaPineTree;
  MegaSpruceTree megaSpruceTree;
};

#endif //EXT_VANILLAGENERATOR_LIB_GENERATOR_OVERWORLD_POPULATORS_MEGATAIGAPOPULATOR_H_
