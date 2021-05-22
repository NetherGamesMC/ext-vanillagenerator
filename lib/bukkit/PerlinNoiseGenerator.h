#ifndef PERLIN_NOISE_GENERATOR_H
#define PERLIN_NOISE_GENERATOR_H

#include "BasePerlinNoiseGenerator.h"

class PerlinNoiseGenerator : public BasePerlinNoiseGenerator {
public:

    float noise(float x, float y, float z, int octaves, float frequency, float amplitude, bool normalized = false) {
        float result = 0.0, amp = 1.0, freq = 1.0, max = 0.0;

        for (auto i = 0; i < octaves; ++i) {
            result += noise3d(x * freq, y * freq, z * freq) * amp;
            max += amp;
            freq *= frequency;
            amp *= amplitude;
        }

        if (normalized) {
            result /= max;
        }

        return result;
    }

};

#endif
