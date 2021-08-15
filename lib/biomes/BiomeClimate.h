#ifndef EXT_VANILLAGENERATOR_LIB_VANILLA_BIOME_H_
#define EXT_VANILLAGENERATOR_LIB_VANILLA_BIOME_H_

#include <map>
#include <cstdint>
#include <lib/noise/octaves/SimplexOctaveGenerator.h>

class BiomeClimate {
 public:
  BiomeClimate(double dTemperature = 0.0, double dHumidity = 0.0, bool bCanRain = false) : temperature(dTemperature), humidity(dHumidity), canRain(bCanRain) {}

  double temperature = 0.0;
  double humidity = 0.0;
  bool canRain = false;

  static Random random;
  static std::map<uint_fast8_t, BiomeClimate> climates;
  static SimplexOctaveGenerator noiseGen;

  const static BiomeClimate defaultClimate;

  /*
   * Return noise-interpolated temperature for the given biome and coordinates.
   */
  static double GetVariatedTemperature(uint_fast8_t biome, int_fast32_t x, int_fast32_t y, int_fast32_t z);

  /*
   * Returns the actual temperature for the current biome.
   */
  static double GetBiomeTemperature(uint_fast8_t biome);

  /*
   * Return the humidity of the biome
   */
  static double GetBiomeHumidity(uint_fast8_t biome);

  /*
   * self-explanatory
   */
  static bool IsWet(uint_fast8_t biome);

  /*
   * self-explanatory
   */
  static bool IsCold(uint_fast8_t biome, int_fast32_t x, int_fast32_t y, int_fast32_t z);

  /*
   * self-explanatory
   */
  static bool IsRainy(uint_fast8_t biome, int_fast32_t x, int_fast32_t y, int_fast32_t z);

  /*
   * self-explanatory
   */
  static bool IsSnowy(uint_fast8_t biome, int_fast32_t x, int_fast32_t y, int_fast32_t z);

  /*
   * initialize biomes objects
   */
  static void Init(bool isUHC);

 private:
  static void RegisterBiome(BiomeClimate climate, const std::vector<uint_fast8_t> &biomeIds);

  static BiomeClimate Get(uint_fast8_t biome);
};

#endif // EXT_VANILLAGENERATOR_LIB_VANILLA_BIOME_H_
