#ifndef SIMPLEX_OCTAVE_GENERATOR_H
#define SIMPLEX_OCTAVE_GENERATOR_H

#include "PerlinOctaveGenerator.h"
#include "lib/glowstone/SimplexNoise.h"

class SimplexOctaveGenerator : public PerlinOctaveGenerator {
public:
    void init(Random *random, int octavesNum, int size_x, int size_y, int size_z) override {
        for (int i = 0; i < octavesNum; ++i) {
            auto simplex = new SimplexNoise();
            simplex->init(random);

            octaves.push_back(simplex);
        }

        this->size_x = size_x;
        this->size_y = size_y;
        this->size_z = size_z;
    }

    std::vector<float>
    getFractalBrownianMotion(float x, float y, float z, float lacunarity, float persistence) override {
        int array_size = size_x * size_y * size_z;

        // Initialize vector array first.
        std::vector<float> noise(array_size);
        for (int i = 0; i < array_size; ++i) {
            noise[i] = 0.0f;
        }


        float freq = 1.0, amp = 1.0;

        for (NoiseGenerator *octave : octaves) {
            auto octavesData = octave->getNoise(x, y, z, size_x, size_y, size_z, x_scale * freq, y_scale * freq,
                                                z_scale * freq, 0.55f / amp);

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
};

#endif
