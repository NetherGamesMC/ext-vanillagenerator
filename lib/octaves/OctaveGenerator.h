//
// Created by User on 23/5/2021.
//

#ifndef OCTAVE_GENERATOR_H
#define OCTAVE_GENERATOR_H

#include "BaseOctaveGenerator.h"

class OctaveGenerator : public BaseOctaveGenerator {
public:

    /**
     * Generates noise for the 3D coordinates using the specified number of
     * octaves and parameters
     *
     * @param x X-coordinate
     * @param y Y-coordinate
     * @param z Z-coordinate
     * @param frequency How much to alter the frequency by each octave
     * @param amplitude How much to alter the amplitude by each octave
     * @param normalized If true, normalize the value to [-1, 1]
     * @return float resulting noise
     */
    float noise(float x, float y, float z, float frequency, float amplitude, bool normalized) {
        float result = 0.0, amp = 1.0, freq = 1.0, max = 0.0;

        x *= getXScale();
        y *= getYScale();
        z *= getZScale();

        for (NoiseGenerator *octave : this->octaves) {
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
};

#endif
