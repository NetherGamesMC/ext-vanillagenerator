#ifndef PERLIN_OCTAVE_GENERATOR_H
#define PERLIN_OCTAVE_GENERATOR_H

#include <vector>
#include "OctaveGenerator.h"
#include "lib/glowstone/PerlinNoise.h"

class PerlinOctaveGenerator : public OctaveGenerator {
public:

    virtual void init(Random *random, int octavesNum, int size_xv, int size_yv, int size_zv) {
        for (int i = 0; i < octavesNum; ++i) {
            auto perlin = new PerlinNoise();
            perlin->init(random);

            octaves.push_back(perlin);
        }

        size_x = size_xv;
        size_y = size_yv;
        size_z = size_zv;
    }

    int getSizeX() {
        return size_x;
    }

    int getSizeY() {
        return size_y;
    }

    int getSizeZ() {
        return size_z;
    }

    /**
     * Generates multiple layers of noise.
     *
     * @param float x the starting X coordinate
     * @param float y the starting Y coordinate
     * @param float z the starting Z coordinate
     * @param float lacunarity layer n's frequency as a fraction of layer {@code n - 1}'s frequency
     * @param float persistence layer n's amplitude as a multiple of layer {@code n - 1}'s amplitude
     * @return float[] the noise array
     */
    virtual std::vector<float>
    getFractalBrownianMotion(float x, float y, float z, float lacunarity, float persistence) {
        int array_size = size_x * size_y * size_z;

        // Initialize vector array first.
        std::vector<float> noise(array_size);
        for (int i = 0; i < array_size; ++i) {
            noise[i] = 0.0f;
        }

        int lx, lz;
        float freq = 1.0, amp = 1.0;
        float dx, dy, dz;

        x *= x_scale;
        y *= y_scale;
        z *= z_scale;

        // fBm
        // the noise have to be periodic over x and z axis: otherwise it can go crazy with high
        // input, leading to strange oddities in terrain generation like the old minecraft farland symptoms.
        for (NoiseGenerator *octave : octaves) {
            dx = x * freq;
            dz = z * freq;
            // compute integer part
            lx = floor(dx);
            lz = floor(dz);
            // compute fractional part
            dx -= lx;
            dz -= lz;
            // wrap integer part to 0..16777216
            lx %= 16777216;
            lz %= 16777216;
            // add to fractional part
            dx += lx;
            dz += lz;

            dy = y * freq;

            auto octavesData = octave->getNoise(dx, dy, dz, size_x, size_y, size_z, x_scale * freq, y_scale * freq,
                                                z_scale * freq, amp);

            int id = 0;
            for (const float &octaveVal : octavesData) {
                noise[id] = noise[id] + octaveVal;

                id++;
            }

            freq *= lacunarity;
            amp *= persistence;
        }

        return noise;
    }

protected:
    int size_x;
    int size_y;
    int size_z;
};

#endif
