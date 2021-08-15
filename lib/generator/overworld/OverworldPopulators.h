#ifndef EXT_VANILLAGENERATOR_LIB_GENERATOR_OVERWORLDPOPULATORS_H_
#define EXT_VANILLAGENERATOR_LIB_GENERATOR_OVERWORLDPOPULATORS_H_

#include <lib/generator/overworld/populators/BiomePopulator.h>
#include <lib/generator/overworld/populators/PlainsPopulator.h>
#include <lib/generator/overworld/populators/SunflowerPlainsPopulator.h>
#include <lib/generator/overworld/populators/ForestPopulator.h>
#include <lib/generator/overworld/populators/BirchForestPopulator.h>
#include <lib/generator/overworld/populators/BirchForestMountainsPopulator.h>
#include <lib/generator/overworld/populators/FlowerForestPopulator.h>
#include <lib/generator/overworld/populators/DesertPopulator.h>
#include <lib/generator/overworld/populators/DesertMountainsPopulator.h>
#include <lib/generator/overworld/populators/IcePlainsPopulator.h>
#include <lib/generator/overworld/populators/IcePlainsSpikesPopulator.h>
#include <lib/generator/overworld/populators/SavannaPopulator.h>
#include <lib/generator/overworld/populators/SavannaMountainsPopulator.h>
#include <lib/generator/overworld/populators/TaigaPopulator.h>
#include <lib/generator/overworld/populators/JunglePopulator.h>
#include <lib/generator/overworld/populators/RoofedForestPopulator.h>
#include <lib/generator/overworld/populators/SwamplandPopulator.h>
#include <lib/generator/overworld/populators/JungleEdgePopulator.h>
#include <lib/generator/overworld/populators/MegaTaigaPopulator.h>
#include <lib/generator/overworld/populators/MegaSpruceTaigaPopulator.h>

class OverworldPopulator : public Populator {
 public:
  OverworldPopulator();

  ~OverworldPopulator();

  void RegisterBiomePopulator(BiomePopulator &populator);

  void Populate(ChunkManager &world, Random &random, int_fast32_t chunkX, int_fast32_t chunkZ) override;

 private:
  std::map<uint_fast8_t, BiomePopulator&> biomePopulators;

  BiomePopulator defaultPopulator;
  PlainsPopulator plainsPopulator;
  SunflowerPlainsPopulator sunflowerPlainsPopulator;
  ForestPopulator forestPopulator;
  BirchForestPopulator birchForestPopulator;
  BirchForestMountainsPopulator birchForestMountainsPopulator;
  RoofedForestPopulator roofedForestPopulator;
  FlowerForestPopulator flowerForestPopulator;
  DesertPopulator desertPopulator;
  DesertMountainsPopulator desertMountainsPopulator;
  JunglePopulator junglePopulator;
  JungleEdgePopulator jungleEdgePopulator;
  SwamplandPopulator swamplandPopulator;
  TaigaPopulator taigaPopulator;
  MegaTaigaPopulator megaTaigaPopulator;
  MegaSpruceTaigaPopulator megaSpruceTaigaPopulator;
  IcePlainsPopulator icePlainsPopulator;
  IcePlainsSpikesPopulator icePlainsSpikesPopulator;
  SavannaPopulator savannaPopulator;
  SavannaMountainsPopulator savannaMountainsPopulator;
};

#endif // EXT_VANILLAGENERATOR_LIB_GENERATOR_OVERWORLDPOPULATORS_H_
