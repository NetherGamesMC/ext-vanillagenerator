#ifndef EXT_VANILLAGENERATOR_LIB_GENERATOR_OVERWORLD_POPULATORS_JUNGLEPOPULATOR_H_
#define EXT_VANILLAGENERATOR_LIB_GENERATOR_OVERWORLD_POPULATORS_JUNGLEPOPULATOR_H_

#include <lib/generator/overworld/decorators/MelonDecorator.h>
#include <lib/generator/object/trees/defaults/JungleBush.h>
#include <lib/generator/object/trees/defaults/MegaJungleTree.h>
#include "BiomePopulator.h"

class JunglePopulator : public BiomePopulator {
 public:
  void InitPopulators() override;

  [[nodiscard]] std::vector<uint_fast8_t> GetBiomes() const override;

  void OnGroundPopulation(ChunkManager &world, Random &random, int_fast32_t chunkX, int_fast32_t chunkZ) override;
 protected:
  MelonDecorator decorator;

  JungleBush jungleBush;
  MegaJungleTree megaJungleTree;
  CocoaTree cocoaTree;
};

#endif //EXT_VANILLAGENERATOR_LIB_GENERATOR_OVERWORLD_POPULATORS_JUNGLEPOPULATOR_H_
