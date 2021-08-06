#ifndef EXT_NOISELIB_LIB_GENERATOR_POPULATORS_OREPOPULATOR_H_
#define EXT_NOISELIB_LIB_GENERATOR_POPULATORS_OREPOPULATOR_H_

#include <lib/objects/random/Random.h>
#include "Populator.h"

struct OreType {
  MinecraftBlock blockType;

  int minY;
  int maxY;
  int amount;
  int total;
  int targetType = 1;

  /*
   * Generates a random height at which a vein of this ore can spawn.
   */
  int GetRandomHeight(Random &random) const;
};

class OrePopulator : public Populator {
 public:
  OrePopulator();

  void Populate(ChunkManager &world, Random &random, int_fast64_t chunkX, int_fast64_t chunkZ) override;

 protected:
  void AddOre(const OreType &ore);

 private:
  std::vector<OreType> ores;
};

#endif //EXT_NOISELIB_LIB_GENERATOR_POPULATORS_OREPOPULATOR_H_
