#include <map>
#include <cstdint>
#include <vector>
#include <lib/objects/constants/BiomeList.h>
#include "BiomeHeightManager.h"

std::map<uint_fast8_t, BiomeHeightManager> BiomeHeightManager::heights;
BiomeHeightManager BiomeHeightManager::defaultHeight = {0.1, 0.2};

BiomeHeightManager BiomeHeightManager::Get(int_fast32_t biome) {
  if (heights.find(biome) != heights.end()) {
    return heights[biome];
  }

  return defaultHeight;
}

void BiomeHeightManager::RegisterBiome(BiomeHeightManager climate, const std::vector<uint_fast8_t> &biomeIds) {
  for (uint_fast8_t biomeId : biomeIds) {
    heights.insert({biomeId, climate});
  }
}

void BiomeHeightManager::Init(bool isUHC) {
  if (isUHC) {
    RegisterBiome({0.2, 0.2}, {TAIGA, COLD_TAIGA, MEGA_TAIGA});
    RegisterBiome({0.45, 0.3}, {ICE_MOUNTAINS, DESERT_HILLS, FOREST_HILLS, TAIGA_HILLS, SMALL_MOUNTAINS, BIRCH_FOREST_HILLS, COLD_TAIGA_HILLS, MESA_PLATEAU_FOREST_MOUNTAINS, MESA_PLATEAU_MOUNTAINS});
    RegisterBiome({0.2, 0.4}, {BIRCH_FOREST_MOUNTAINS, ROOFED_FOREST_MOUNTAINS});
    RegisterBiome({0.3, 0.4}, {TAIGA_MOUNTAINS, COLD_TAIGA_MOUNTAINS});
  } else {
    RegisterBiome({-1.0, 0.1}, {OCEAN, FROZEN_OCEAN});
    RegisterBiome({-1.8, 0.1}, {DEEP_OCEAN});
    RegisterBiome({0.2, 0.2}, {TAIGA, COLD_TAIGA, MEGA_TAIGA});
    RegisterBiome({-0.2, 0.1}, {SWAMPLAND});
    RegisterBiome({0.45, 0.3}, {ICE_MOUNTAINS, DESERT_HILLS, FOREST_HILLS, TAIGA_HILLS, SMALL_MOUNTAINS, JUNGLE_HILLS, BIRCH_FOREST_HILLS, COLD_TAIGA_HILLS, MEGA_TAIGA_HILLS, MESA_PLATEAU_FOREST_MOUNTAINS, MESA_PLATEAU_MOUNTAINS});
    RegisterBiome({-0.1, 0.3}, {SWAMPLAND_MOUNTAINS});
    RegisterBiome({0.2, 0.4}, {JUNGLE_MOUNTAINS, JUNGLE_EDGE_MOUNTAINS, BIRCH_FOREST_MOUNTAINS, ROOFED_FOREST_MOUNTAINS});
    RegisterBiome({0.3, 0.4}, {TAIGA_MOUNTAINS, COLD_TAIGA_MOUNTAINS, MEGA_SPRUCE_TAIGA, MEGA_SPRUCE_TAIGA_HILLS});
  }

  RegisterBiome({-0.5, 0.0}, {RIVER, FROZEN_RIVER});
  RegisterBiome({0.0, 0.025}, {BEACH, COLD_BEACH, MUSHROOM_SHORE});
  RegisterBiome({0.1, 0.8}, {STONE_BEACH});
  RegisterBiome({0.125, 0.05}, {DESERT, ICE_PLAINS, SAVANNA});

  RegisterBiome({1.0, 0.5}, {EXTREME_HILLS, EXTREME_HILLS_PLUS, EXTREME_HILLS_MOUNTAINS, EXTREME_HILLS_PLUS_MOUNTAINS});
  RegisterBiome({0.2, 0.3}, {MUSHROOM_ISLAND});

  RegisterBiome({1.5, 0.025}, {SAVANNA_PLATEAU, MESA_PLATEAU_FOREST, MESA_PLATEAU});
  RegisterBiome({0.275, 0.25}, {DESERT_MOUNTAINS});
  RegisterBiome({0.525, 0.55}, {ICE_PLAINS_SPIKES});
  RegisterBiome({0.55, 0.5}, {BIRCH_FOREST_HILLS_MOUNTAINS});
  RegisterBiome({0.1, 0.4}, {FLOWER_FOREST});
  RegisterBiome({0.4125, 1.325}, {SAVANNA_MOUNTAINS});
  RegisterBiome({1.1, 1.3125}, {SAVANNA_PLATEAU_MOUNTAINS});
}