#ifndef EXT_NOISELIB_SIMPLEX_OCTAVE_GENERATOR_H
#define EXT_NOISELIB_SIMPLEX_OCTAVE_GENERATOR_H

#include "lib/octaves/BaseOctaveGenerator.h"
#include "lib/bukkit/SimplexNoiseGenerator.h"

/**
 * Bukkit Simplex Noise Generator used for biomes grid noise map layer.
 */
class BukkitSimplexOctaveGenerator : public BaseOctaveGenerator {
public:

    void init(Random *random, int octavesNum) {
        for (int i = 0; i < octavesNum; ++i) {
            auto perlin = new SimplexNoiseGenerator();
            perlin->init(random);

            octaves.push_back(perlin);
        }
    }

    void setScale(float scale) override {
        BaseOctaveGenerator::setScale(scale);

        w_scale = scale;
    }

    /**
     * Generates noise for the 3D coordinates using the specified number of
     * octaves and parameters
     *
     * @param x X-coordinate
     * @param y Y-coordinate
     * @param z Z-coordinate
     * @param frequency How much to alter the frequency by each octave
     * @param amplitude How much to alter the amplitude by each octave
     * @param bool normalized If true, normalize the value to [-1, 1]
     * @return The resulting noise in float
     */
    float octaveNoise(float x, float y, float z, float frequency, float amplitude, bool normalized) {
        float result = 0.0, amp = 1.0, freq = 1.0, max = 0.0;

        x *= this->x_scale;
        y *= this->y_scale;
        z *= this->z_scale;

        for (NoiseGenerator *octave : octaves) {
            result += octave->noise3d(x * freq, y * freq, z * freq) * amp;
            max += amp;
            freq *= frequency;
            amp *= amplitude;
        }

        if (normalized) {
            result /= max;
        }

        return result;
    }

    /**
     * Generates noise for the 3D coordinates using the specified number of
     * octaves and parameters
     *
     * @param float x X-coordinate
     * @param float y Y-coordinate
     * @param float z Z-coordinate
     * @param float w W-coordinate
     * @param float frequency How much to alter the frequency by each octave
     * @param float amplitude How much to alter the amplitude by each octave
     * @param bool normalized If true, normalize the value to [-1, 1]
     * @return float resulting noise
     */
    float noise(float x, float y, float z, float w, float frequency, float amplitude, bool normalized = false) {
        float result = 0.0, amp = 1.0, freq = 1.0, max = 0.0;

        x *= this->x_scale;
        y *= this->y_scale;
        z *= this->z_scale;
        w *= this->w_scale;

        for (NoiseGenerator *octave : octaves) {
            result += octave->noise(x * freq, y * freq, z * freq, w * freq) * amp;
            max += amp;
            freq *= frequency;
            amp *= amplitude;
        }

        if (normalized) {
            result /= max;
        }

        return result;
    }

private:
    float w_scale = 1.0;
};

#endif
