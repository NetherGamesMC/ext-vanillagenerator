#ifndef EXT_NOISELIB_LIB_GENERATOR_POPULATORS_OREPOPULATOR_H_
#define EXT_NOISELIB_LIB_GENERATOR_POPULATORS_OREPOPULATOR_H_

#include <lib/random/Random.h>
#include "Populator.h"

struct OreType {
  MinecraftBlock block_type;

  int min_y;
  int max_y;
  int amount;
  int total;
  int target_type = 1;

  /*
   * Generates a random height at which a vein of this ore can spawn.
   */
  int getRandomHeight(Random &random) const;
};

class OrePopulator : public Populator {
 public:
  OrePopulator();

  void Populate(SimpleChunkManager &world, Random &random, int_fast64_t chunkX, int_fast64_t chunkZ) override;

 protected:
  void addOre(const OreType &ore);

 private:
  std::vector<OreType> ores;
};

#endif //EXT_NOISELIB_LIB_GENERATOR_POPULATORS_OREPOPULATOR_H_
