#ifndef EXT_VANILLAGENERATOR_LIB_GENERATOR_NETHER_POPULATOR_NETHEROREPOPULATOR_H_
#define EXT_VANILLAGENERATOR_LIB_GENERATOR_NETHER_POPULATOR_NETHEROREPOPULATOR_H_

#include <lib/generator/overworld/populators/OrePopulator.h>

class NetherOrePopulator : public OrePopulator {
 public:
  NetherOrePopulator(int_fast32_t worldHeight);
};

#endif //EXT_VANILLAGENERATOR_LIB_GENERATOR_NETHER_POPULATOR_NETHEROREPOPULATOR_H_
