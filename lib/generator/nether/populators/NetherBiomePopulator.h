#ifndef EXT_VANILLAGENERATOR_LIB_GENERATOR_NETHER_POPULATOR_NETHERBIOMEPOPULATOR_H_
#define EXT_VANILLAGENERATOR_LIB_GENERATOR_NETHER_POPULATOR_NETHERBIOMEPOPULATOR_H_

#include <lib/generator/overworld/populators/BiomePopulator.h>
#include <lib/generator/nether/decorators/FireDecorator.h>
#include <lib/generator/nether/decorators/GlowstoneDecorator.h>
#include <lib/generator/nether/decorators/NetherMushroomDecorator.h>
#include <lib/generator/nether/populators/NetherOrePopulator.h>
#include <lib/objects/constants/BiomeList.h>

class NetherBiomePopulator : public BiomePopulator{
 public:
  NetherBiomePopulator();

  void Populate(ChunkManager &world, Random &random, int_fast32_t chunkX, int_fast32_t chunkZ) override;

  void InitPopulators() override;
 private:
  NetherOrePopulator orePopulator_ = {128};
  FireDecorator fireDecorator_;
  GlowstoneDecorator glowstoneDecorator_ = {true};
  GlowstoneDecorator glowstoneDecorator2_ = {false};
  NetherMushroomDecorator brownMushroomDecorator_ = {BROWN_MUSHROOM};
  NetherMushroomDecorator redMushroomDecorator_ = {RED_MUSHROOM};
};

#endif //EXT_VANILLAGENERATOR_LIB_GENERATOR_NETHER_POPULATOR_NETHERBIOMEPOPULATOR_H_