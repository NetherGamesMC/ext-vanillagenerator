#ifndef EXT_VANILLAGENERATOR_LIB_GENERATOR_POPULATORS_OREPOPULATOR_H_
#define EXT_VANILLAGENERATOR_LIB_GENERATOR_POPULATORS_OREPOPULATOR_H_

#include <lib/objects/random/Random.h>
#include "lib/generator/Populator.h"

struct OreType {
  MinecraftBlock blockType;

  int_fast32_t minY;
  int_fast32_t maxY;
  int_fast32_t amount;
  int_fast32_t total;
  int_fast32_t targetType = 1;

  /*
   * Generates a random height at which a vein of this ore can spawn.
   */
  int_fast32_t GetRandomHeight(Random &random) const;
};

class OrePopulator : public Populator {
 public:
  OrePopulator();

  void Populate(ChunkManager &world, Random &random, int_fast32_t chunkX, int_fast32_t chunkZ) override;

 protected:
  void AddOre(OreType ore);

 private:
  std::vector<OreType> ores;
};

#endif //EXT_VANILLAGENERATOR_LIB_GENERATOR_POPULATORS_OREPOPULATOR_H_
