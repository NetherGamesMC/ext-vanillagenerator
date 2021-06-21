#ifndef EXT_NOISELIB_LIB_VANILLA_BIOME_H_
#define EXT_NOISELIB_LIB_VANILLA_BIOME_H_

struct BiomeClimate {
  double temperature = 0.0;
  double humidity = 0.0;
  bool canRain = false;
};

/*
 * Return noise-interpolated temperature for the given biome and coordinates.
 */
double getVariatedTemperature(uint_fast8_t biome, int x, int y, int z);

/*
 * Returns the actual temperature for the current biome.
 */
double getBiomeTemperature(uint_fast8_t biome);

/*
 * Return the humidity of the biome
 */
double getBiomeHumidity(uint_fast8_t biome);

/*
 * self-explanatory
 */
bool isWet(uint_fast8_t biome);

/*
 * self-explanatory
 */
bool isCold(uint_fast8_t biome, int x, int y, int z);

/*
 * self-explanatory
 */
bool isRainy(uint_fast8_t biome, int x, int y, int z);

/*
 * self-explanatory
 */
bool isSnowy(uint_fast8_t biome, int x, int y, int z);

/*
 * initialize biomes objects
 */
void init_biomes();

#endif // EXT_NOISELIB_LIB_VANILLA_BIOME_H_
