#ifndef EXT_NOISELIB_LIB_VANILLA_BIOME_H_
#define EXT_NOISELIB_LIB_VANILLA_BIOME_H_

#include <cstdint>

struct BiomeClimate {
  double temperature = 0.0;
  double humidity = 0.0;
  bool canRain = false;
};

namespace Biome {

/*
 * Return noise-interpolated temperature for the given biome and coordinates.
 */
double GetVariatedTemperature(uint_fast8_t biome, int_fast64_t x, int_fast32_t y, int_fast64_t z);

/*
 * Returns the actual temperature for the current biome.
 */
double GetBiomeTemperature(uint_fast8_t biome);

/*
 * Return the humidity of the biome
 */
double GetBiomeHumidity(uint_fast8_t biome);

/*
 * self-explanatory
 */
bool IsWet(uint_fast8_t biome);

/*
 * self-explanatory
 */
bool IsCold(uint_fast8_t biome, int_fast64_t x, int_fast32_t y, int_fast64_t z);

/*
 * self-explanatory
 */
bool IsRainy(uint_fast8_t biome, int_fast64_t x, int_fast32_t y, int_fast64_t z);

/*
 * self-explanatory
 */
bool IsSnowy(uint_fast8_t biome, int_fast64_t x, int_fast32_t y, int_fast64_t z);

/*
 * initialize biomes objects
 */
void Init(bool isUHC);

/*
 * Deletes all related pointers to this instance.
 */
void Clean();

}

#endif // EXT_NOISELIB_LIB_VANILLA_BIOME_H_
