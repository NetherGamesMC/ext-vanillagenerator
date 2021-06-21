#include <map>
#include <lib/vanilla/Biome.h>
#include <lib/vanilla/VanillaNoise.h>
#include <lib/pocketmine/BiomeList.h>

static SimplexOctaveGenerator *noise_gen = nullptr;

static std::map<uint_fast8_t, BiomeClimate> *climates;

BiomeClimate get(uint_fast8_t biome) {
  if (climates->find(biome) != climates->end()) {
    return (*climates)[biome];
  }

  return {0.5, 0.5, true};
}

double getVariatedTemperature(uint_fast8_t biome, int x, int y, int z) {
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

bool isCold(uint_fast8_t biome, int x, int y, int z) {
  return getVariatedTemperature(biome, x, y, z) < 0.15;
}

bool isRainy(uint_fast8_t biome, int x, int y, int z) {
  return get(biome).canRain && !isCold(biome, x, y, z);
}

bool isSnowy(uint_fast8_t biome, int x, int y, int z) {
  return get(biome).canRain && isCold(biome, x, y, z);
}

void registerBiome(const BiomeClimate climate, uint_fast8_t biomeId) {
  climates->insert({biomeId, climate});
}

void init_biomes() {
  if (noise_gen != nullptr) {
    return;
  }

  Random random = Random(1234);
  noise_gen = new SimplexOctaveGenerator(random, 1, 0, 0, 0);

  BiomeClimate climate{0.8, 0.4, true};
  registerBiome(climate, PLAINS);
  registerBiome(climate, SUNFLOWER_PLAINS);
  registerBiome(climate, BEACH);

  climate = {2.0, 0.0, false};
  registerBiome(climate, DESERT);
  registerBiome(climate, DESERT_HILLS);
  registerBiome(climate, DESERT_MOUNTAINS);
  registerBiome(climate, MESA);
  registerBiome(climate, MESA_BRYCE);
  registerBiome(climate, MESA_PLATEAU);
  registerBiome(climate, MESA_PLATEAU_FOREST);
  registerBiome(climate, MESA_PLATEAU_MOUNTAINS);
  registerBiome(climate, MESA_PLATEAU_FOREST_MOUNTAINS);
  registerBiome(climate, HELL);

  climate = {0.2, 0.3, true};
  registerBiome(climate, EXTREME_HILLS);
  registerBiome(climate, EXTREME_HILLS_PLUS);
  registerBiome(climate, EXTREME_HILLS_MOUNTAINS);
  registerBiome(climate, EXTREME_HILLS_PLUS_MOUNTAINS);
  registerBiome(climate, STONE_BEACH);
  registerBiome(climate, SMALL_MOUNTAINS);

  climate = {0.7, 0.8, true};
  registerBiome(climate, FOREST);
  registerBiome(climate, FOREST_HILLS);
  registerBiome(climate, FLOWER_FOREST);
  registerBiome(climate, ROOFED_FOREST);
  registerBiome(climate, ROOFED_FOREST_MOUNTAINS);

  climate = {0.6, 0.6, true};
  registerBiome(climate, BIRCH_FOREST);
  registerBiome(climate, BIRCH_FOREST_HILLS);
  registerBiome(climate, BIRCH_FOREST_MOUNTAINS);
  registerBiome(climate, BIRCH_FOREST_HILLS_MOUNTAINS);

  climate = {0.6, 0.6, true};
  registerBiome(climate, TAIGA);
  registerBiome(climate, TAIGA_HILLS);
  registerBiome(climate, TAIGA_MOUNTAINS);
  registerBiome(climate, MEGA_SPRUCE_TAIGA);
  registerBiome(climate, MEGA_SPRUCE_TAIGA_HILLS);

  climate = {0.8, 0.9, true};
  registerBiome(climate, SWAMPLAND);
  registerBiome(climate, SWAMPLAND_MOUNTAINS);

  climate = {0.0, 0.5, true};
  registerBiome(climate, ICE_PLAINS);
  registerBiome(climate, ICE_MOUNTAINS);
  registerBiome(climate, ICE_PLAINS_SPIKES);
  registerBiome(climate, FROZEN_RIVER);
  registerBiome(climate, FROZEN_OCEAN);

  climate = {0.9, 1.0, true};
  registerBiome(climate, MUSHROOM_ISLAND);
  registerBiome(climate, MUSHROOM_SHORE);

  registerBiome({0.05, 0.3, true}, COLD_BEACH);

  climate = {0.95, 0.9, true};
  registerBiome(climate, JUNGLE_HILLS);
  registerBiome(climate, JUNGLE_MOUNTAINS);

  climate = {0.95, 0.8, true};
  registerBiome(climate, JUNGLE_EDGE);
  registerBiome(climate, JUNGLE_EDGE_MOUNTAINS);

  climate = {-0.5, 0.4, true};
  registerBiome(climate, COLD_TAIGA);
  registerBiome(climate, COLD_TAIGA_HILLS);
  registerBiome(climate, COLD_TAIGA_MOUNTAINS);

  climate = {0.3, 0.8, true};
  registerBiome(climate, MEGA_TAIGA);
  registerBiome(climate, MEGA_TAIGA_HILLS);

  registerBiome({1.2, 0.0, false}, SAVANNA);
  registerBiome({1.1, 0.0, false}, SAVANNA_MOUNTAINS);
  registerBiome({1.0, 0.0, false}, SAVANNA_PLATEAU);
  registerBiome({0.5, 0.0, false}, SAVANNA_PLATEAU_MOUNTAINS);
  registerBiome({0.5, 0.5, false}, SKY);
}