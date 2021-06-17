#ifndef EXT_NOISELIB_BIOME_H
#define EXT_NOISELIB_BIOME_H

class BiomeClimate {
public:
    BiomeClimate(double temperature, double humidity, bool rainy) : temp(temperature), humid(humidity), rain(rainy) {}

    double getTemperature() const {
        return temp;
    }

    double getHumidity() const {
        return humid;
    }

    bool isRainy() const {
        return rain;
    }

private:
    double temp;
    double humid;
    bool rain;
};

/**
 * Return noise-interpolated temperature for the given biome and coordinates.
 */
double getVariatedTemperature(int biome, int x, int y, int z);

/**
 * Returns the actual temperature for the current biome.
 */
double getBiomeTemperature(int biome);

/**
 * Return the humidity of the biome
 */
double getBiomeHumidity(int biome);

/**
 * self-explanatory
 */
bool isWet(int biome);

/**
 * self-explanatory
 */
bool isCold(int biome, int x, int y, int z);

/**
 * self-explanatory
 */
bool isRainy(int biome, int x, int y, int z);

/**
 * self-explanatory
 */
bool isSnowy(int biome, int x, int y, int z);

/**
 * initialize biomes objects
 */
void init_biomes();

#endif //EXT_NOISELIB_BIOME_H
