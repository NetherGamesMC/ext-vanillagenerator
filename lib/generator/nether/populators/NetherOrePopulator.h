#ifndef EXT_NOISELIB_LIB_GENERATOR_NETHER_POPULATOR_NETHEROREPOPULATOR_H_
#define EXT_NOISELIB_LIB_GENERATOR_NETHER_POPULATOR_NETHEROREPOPULATOR_H_

#include <lib/generator/overworld/populators/OrePopulator.h>

class NetherOrePopulator : public OrePopulator {
 public:
  NetherOrePopulator(int_fast32_t worldHeight);
};

#endif //EXT_NOISELIB_LIB_GENERATOR_NETHER_POPULATOR_NETHEROREPOPULATOR_H_
