#include "BiomeClimate.h"
#include <lib/objects/constants/BiomeList.h>

Random BiomeClimate::random = Random(1234);
SimplexOctaveGenerator BiomeClimate::noiseGen = SimplexOctaveGenerator(random, 1, 0, 0, 0);
std::map<uint_fast8_t, BiomeClimate> BiomeClimate::climates;

const BiomeClimate BiomeClimate::defaultClimate = {0.8, 0.4, true};

BiomeClimate BiomeClimate::Get(uint_fast8_t biome) {
  if (climates.find(biome) != climates.end()) {
    return climates[biome];
  }

  return defaultClimate;
}

double BiomeClimate::GetVariatedTemperature(uint_fast8_t biome, int_fast32_t x, int_fast32_t y, int_fast32_t z) {
  double temp, variation;

  temp = Get(biome).temperature;
  if (y > 64) {
    variation = noiseGen.Noise(static_cast<double>(x), static_cast<double>(z), 0, 0.5, 2.0, false) * 4.0;

    return temp - (variation + (float) (y - 64)) * 0.05 / 30.0;
  }

  return temp;
}

double BiomeClimate::GetBiomeTemperature(uint_fast8_t biome) {
  return Get(biome).temperature;
}

double BiomeClimate::GetBiomeHumidity(uint_fast8_t biome) {
  return Get(biome).humidity;
}

bool BiomeClimate::IsWet(uint_fast8_t biome) {
  return GetBiomeHumidity(biome) > 0.85;
}

bool BiomeClimate::IsCold(uint_fast8_t biome, int_fast32_t x, int_fast32_t y, int_fast32_t z) {
  return GetVariatedTemperature(biome, x, y, z) < 0.15;
}

bool BiomeClimate::IsRainy(uint_fast8_t biome, int_fast32_t x, int_fast32_t y, int_fast32_t z) {
  return Get(biome).canRain && !IsCold(biome, x, y, z);
}

bool BiomeClimate::IsSnowy(uint_fast8_t biome, int_fast32_t x, int_fast32_t y, int_fast32_t z) {
  return Get(biome).canRain && IsCold(biome, x, y, z);
}

void BiomeClimate::Init(bool isUHC) {
  if (!climates.empty()) {
    return;
  }

  RegisterBiome({0.8, 0.4, true}, {PLAINS, SUNFLOWER_PLAINS, BEACH});
  RegisterBiome({2.0, 0.0, false}, {DESERT, DESERT_HILLS, DESERT_MOUNTAINS, MESA, MESA_BRYCE, MESA_PLATEAU, MESA_PLATEAU_FOREST, MESA_PLATEAU_MOUNTAINS, MESA_PLATEAU_FOREST_MOUNTAINS, HELL});
  RegisterBiome({0.2, 0.3, true}, {EXTREME_HILLS, EXTREME_HILLS_PLUS, EXTREME_HILLS_MOUNTAINS, EXTREME_HILLS_PLUS_MOUNTAINS, STONE_BEACH, SMALL_MOUNTAINS});
  RegisterBiome({0.7, 0.8, true}, {FOREST, FOREST_HILLS, FLOWER_FOREST, ROOFED_FOREST, ROOFED_FOREST_MOUNTAINS});
  RegisterBiome({0.6, 0.6, true}, {BIRCH_FOREST, BIRCH_FOREST_HILLS, BIRCH_FOREST_MOUNTAINS, BIRCH_FOREST_HILLS_MOUNTAINS});

  if (isUHC) {
    RegisterBiome({0.25, 0.8, true}, {TAIGA, TAIGA_HILLS, TAIGA_MOUNTAINS});
    RegisterBiome({0.0, 0.5, true}, {ICE_PLAINS, ICE_MOUNTAINS, ICE_PLAINS_SPIKES, FROZEN_RIVER});
  } else {
    RegisterBiome({0.25, 0.8, true}, {TAIGA, TAIGA_HILLS, TAIGA_MOUNTAINS, MEGA_SPRUCE_TAIGA, MEGA_SPRUCE_TAIGA_HILLS});
    RegisterBiome({0.8, 0.9, true}, {SWAMPLAND, SWAMPLAND_MOUNTAINS});
    RegisterBiome({0.0, 0.5, true}, {ICE_PLAINS, ICE_MOUNTAINS, ICE_PLAINS_SPIKES, FROZEN_RIVER, FROZEN_OCEAN});
    RegisterBiome({0.95, 0.9, true}, {JUNGLE_HILLS, JUNGLE_MOUNTAINS});
    RegisterBiome({0.95, 0.8, true}, {JUNGLE_EDGE, JUNGLE_EDGE_MOUNTAINS});
    RegisterBiome({0.3, 0.8, true}, {MEGA_TAIGA, MEGA_TAIGA_HILLS});
  }

  RegisterBiome({0.9, 1.0, true}, {MUSHROOM_ISLAND, MUSHROOM_SHORE});
  RegisterBiome({0.05, 0.3, true}, {COLD_BEACH});
  RegisterBiome({-0.5, 0.4, true}, {COLD_TAIGA, COLD_TAIGA_HILLS, COLD_TAIGA_MOUNTAINS});
  RegisterBiome({1.2, 0.0, false}, {SAVANNA});
  RegisterBiome({1.1, 0.0, false}, {SAVANNA_MOUNTAINS});
  RegisterBiome({1.0, 0.0, false}, {SAVANNA_PLATEAU});
  RegisterBiome({0.5, 0.0, false}, {SAVANNA_PLATEAU_MOUNTAINS});
  RegisterBiome({0.5, 0.5, false}, {SKY});
}

void BiomeClimate::RegisterBiome(BiomeClimate climate, const std::vector<uint_fast8_t> &biomeIds) {
  for (uint_fast8_t biomeId : biomeIds) {
    climates.insert({biomeId, climate});
  }
}