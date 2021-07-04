#include <map>
#include <lib/objects/Biome.h>
#include <lib/pocketmine/BiomeList.h>
#include <lib/vanilla/octaves/SimplexOctaveGenerator.h>
#include <cstdio>

namespace Biome {

thread_local SimplexOctaveGenerator *noise_gen = nullptr;
thread_local std::map<uint_fast8_t, BiomeClimate> *climates = nullptr;
thread_local BiomeClimate *defaultClimate = nullptr;

BiomeClimate get(uint_fast8_t biome) {
  if (climates->find(biome) != climates->end()) {
    return (*climates)[biome];
  }

  return (*defaultClimate);
}

double getVariatedTemperature(uint_fast8_t biome, int_fast64_t x, int_fast32_t y, int_fast64_t z) {
  double temp, variation;

  temp = get(biome).temperature;
  if (y > 64) {
    variation = noise_gen->noise(x, z, 0, 0.5, 2.0, false) * 4.0;

    return temp - (variation + (float) (y - 64)) * 0.05 / 30.0;
  }

  return temp;
}

double getBiomeTemperature(uint_fast8_t biome) {
  return get(biome).temperature;
}

double getBiomeHumidity(uint_fast8_t biome) {
  return get(biome).humidity;
}

bool isWet(uint_fast8_t biome) {
  return getBiomeHumidity(biome) > 0.85;
}

bool isCold(uint_fast8_t biome, int_fast64_t x, int_fast32_t y, int_fast64_t z) {
  return getVariatedTemperature(biome, x, y, z) < 0.15;
}

bool isRainy(uint_fast8_t biome, int_fast64_t x, int_fast32_t y, int_fast64_t z) {
  return get(biome).canRain && !isCold(biome, x, y, z);
}

bool isSnowy(uint_fast8_t biome, int_fast64_t x, int_fast32_t y, int_fast64_t z) {
  return get(biome).canRain && isCold(biome, x, y, z);
}

void registerBiome(const BiomeClimate climate, const std::vector<uint_fast8_t> &biomeIds) {
  for (uint_fast8_t biomeId : biomeIds) {
    climates->insert({biomeId, climate});
  }
}

void clean() {
  delete noise_gen;
  delete climates;
  delete defaultClimate;
}

void init() {
  if (noise_gen != nullptr || climates != nullptr || defaultClimate != nullptr) {
    return;
  }

  auto random = Random(1234);
  noise_gen = new SimplexOctaveGenerator(random, 1, 0, 0, 0);
  climates = new std::map<uint_fast8_t, BiomeClimate>;
  defaultClimate = new BiomeClimate{0.8, 0.4, true};

  registerBiome({0.8, 0.4, true}, {PLAINS, SUNFLOWER_PLAINS, BEACH});
  registerBiome({2.0, 0.0, false}, {DESERT, DESERT_HILLS, DESERT_MOUNTAINS,
                                    MESA, MESA_BRYCE, MESA_PLATEAU, MESA_PLATEAU_FOREST,
                                    MESA_PLATEAU_MOUNTAINS, MESA_PLATEAU_FOREST_MOUNTAINS, HELL});
  registerBiome({0.2, 0.3, true}, {EXTREME_HILLS, EXTREME_HILLS_PLUS, EXTREME_HILLS_MOUNTAINS,
                                   EXTREME_HILLS_PLUS_MOUNTAINS, STONE_BEACH, SMALL_MOUNTAINS});
  registerBiome({0.7, 0.8, true}, {FOREST, FOREST_HILLS, FLOWER_FOREST, ROOFED_FOREST, ROOFED_FOREST_MOUNTAINS});
  registerBiome({0.6, 0.6, true}, {BIRCH_FOREST, BIRCH_FOREST_HILLS, BIRCH_FOREST_MOUNTAINS,
                                   BIRCH_FOREST_HILLS_MOUNTAINS});
  registerBiome({0.25, 0.8, true}, {TAIGA, TAIGA_HILLS, TAIGA_MOUNTAINS, MEGA_SPRUCE_TAIGA, MEGA_SPRUCE_TAIGA_HILLS});
  registerBiome({0.8, 0.9, true}, {SWAMPLAND, SWAMPLAND_MOUNTAINS});
  registerBiome({0.0, 0.5, true}, {ICE_PLAINS, ICE_MOUNTAINS, ICE_PLAINS_SPIKES, FROZEN_RIVER, FROZEN_OCEAN});
  registerBiome({0.9, 1.0, true}, {MUSHROOM_ISLAND, MUSHROOM_SHORE});
  registerBiome({0.05, 0.3, true}, {COLD_BEACH});
  registerBiome({0.95, 0.9, true}, {JUNGLE_HILLS, JUNGLE_MOUNTAINS});
  registerBiome({0.95, 0.8, true}, {JUNGLE_EDGE, JUNGLE_EDGE_MOUNTAINS});
  registerBiome({-0.5, 0.4, true}, {COLD_TAIGA, COLD_TAIGA_HILLS, COLD_TAIGA_MOUNTAINS});
  registerBiome({0.3, 0.8, true}, {MEGA_TAIGA, MEGA_TAIGA_HILLS});
  registerBiome({1.2, 0.0, false}, {SAVANNA});
  registerBiome({1.1, 0.0, false}, {SAVANNA_MOUNTAINS});
  registerBiome({1.0, 0.0, false}, {SAVANNA_PLATEAU});
  registerBiome({0.5, 0.0, false}, {SAVANNA_PLATEAU_MOUNTAINS});
  registerBiome({0.5, 0.5, false}, {SKY});
}

}