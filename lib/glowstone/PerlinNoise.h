#ifndef PERLIN_NOISE_H
#define PERLIN_NOISE_H

#include <vector>
#include "lib/bukkit/BasePerlinNoiseGenerator.h"

/**
 * PerlinNoise implementation in C++, this class is translated over
 * from PHP to C++, however the implementation is a bit different than
 * the original PHP code.
 */
class PerlinNoise : public BasePerlinNoiseGenerator {
public:

    void init(Random *rand) override {
        this->offset_x = rand->nextFloat() * 256;
        this->offset_y = rand->nextFloat() * 256;
        this->offset_z = rand->nextFloat() * 256;

        // The only reason why I'm re-implementing the constructor code is that I've read
        // on at least 3 different sources that the permutation table should initially be
        // populated with indices.
        // "The permutation table is his answer to the issue of random numbers.
        // First take an array of decent length, usually 256 values. Fill it sequentially with each
        // number in that range: so index 1 gets 1, index 8 gets 8, index 251 gets 251, etc...
        // Then randomly shuffle the values so you have a table of 256 random values, but only
        // contains the values between 0 and 255."
        // source: https://code.google.com/p/fractalterraingeneration/wiki/Perlin_Noise
        for (int i = 0; i < 256; ++i) {
            this->perm[i] = i;
        }

        for (int i = 0; i < 256; ++i) {
            auto pos = rand->nextBoundedInt(256 - i) + i;
            auto old = this->perm[i];
            this->perm[i] = this->perm[pos];
            this->perm[pos] = old;
            this->perm[i + 256] = this->perm[i];
        }
    }

    std::vector<float>
    getNoise(float x, float y, float z, int size_x, int size_y, int size_z, float scale_x, float scale_y, float scale_z,
             float amplitude) override {

        if (size_y == 1) {
            return get2dNoise(x, z, size_x, size_z, scale_x, scale_z, amplitude);
        }

        return get3dNoise(x, y, z, size_x, size_y, size_z, scale_x, scale_y, scale_z, amplitude);
    }

protected:

    virtual std::vector<float>
    get2dNoise(float x, float z, int size_x, int size_z, float scale_x, float scale_z, float amplitude) {
        php_printf("get2dNoise() Perlin \r\n");

        std::vector<float> vectors;
        for (int i = 0; i < size_x; ++i) {
            float dx = x + offset_x + i * scale_x;
            int floor_x = PerlinNoise::fastfloor(dx);
            int ix = floor_x & 255;
            dx -= (float) floor_x;
            float fx = NoiseGenerator::fade(dx);

            for (auto j = 0; j < size_z; ++j) {
                auto dz = z + offset_z + j * scale_z;
                auto floor_z = PerlinNoise::fastfloor(dz);
                auto iz = floor_z & 255;
                dz -= floor_z;
                auto fz = NoiseGenerator::fade(dz);
                // Hash coordinates of the square corners
                auto a = perm[ix];
                auto aa = perm[a] + iz;
                auto b = perm[ix + 1];
                auto ba = perm[b] + iz;

                auto x1 = lerp(fx, NoiseGenerator::grad(perm[aa], dx, 0, dz),
                               NoiseGenerator::grad(perm[ba], dx - 1, 0, dz));
                auto x2 = lerp(fx, NoiseGenerator::grad(perm[aa + 1], dx, 0, dz - 1),
                               NoiseGenerator::grad(perm[ba + 1], dx - 1, 0, dz - 1));

                vectors.push_back(lerp(fz, x1, x2) * amplitude);
            }
        }

        return vectors;
    }

    virtual std::vector<float>
    get3dNoise(float x, float y, float z, int size_x, int size_y, int size_z, float scale_x, float scale_y,
               float scale_z, float amplitude) {
        php_printf("get3dNoise() Perlin \r\n");
        auto n = -1;
        float x1 = 0, x2 = 0, x3 = 0, x4 = 0;

        std::vector<float> vectors;
        for (auto i = 0; i < size_x; ++i) {
            auto dx = x + offset_x + (float) i * scale_x;
            auto floor_x = PerlinNoise::fastfloor(dx);
            auto ix = floor_x & 255;
            dx -= (float) floor_x;
            auto fx = NoiseGenerator::fade(dx);
            for (auto j = 0; j < size_z; ++j) {
                auto dz = z + offset_z + (float) j * scale_z;
                auto floor_z = PerlinNoise::fastfloor(dz);
                auto iz = floor_z & 255;
                dz -= (float) floor_z;
                auto fz = NoiseGenerator::fade(dz);

                for (auto k = 0; k < size_y; ++k) {
                    auto dy = y + offset_y + (float) k * scale_y;
                    auto floor_y = PerlinNoise::fastfloor(dy);
                    auto iy = floor_y & 255;
                    dy -= (float) floor_y;
                    auto fy = NoiseGenerator::fade(dy);


                    if (k == 0 || iy != n) {
                        n = iy;
                        // Hash coordinates of the cube corners
                        auto a = perm[ix] + iy;
                        auto aa = perm[a] + iz;
                        auto ab = perm[a + 1] + iz;
                        auto b = perm[ix + 1] + iy;
                        auto ba = perm[b] + iz;
                        auto bb = perm[b + 1] + iz;

                        x1 = lerp(fx, NoiseGenerator::grad(perm[aa], dx, dy, dz),
                                  NoiseGenerator::grad(perm[ba], dx - 1, dy, dz));
                        x2 = lerp(fx, NoiseGenerator::grad(perm[ab], dx, dy - 1, dz),
                                  NoiseGenerator::grad(perm[bb], dx - 1, dy - 1, dz));
                        x3 = lerp(fx, NoiseGenerator::grad(perm[aa + 1], dx, dy, dz - 1),
                                  NoiseGenerator::grad(perm[ba + 1], dx - 1, y, dz - 1));
                        x4 = lerp(fx, NoiseGenerator::grad(perm[ab + 1], dx, dy - 1, dz - 1),
                                  NoiseGenerator::grad(perm[bb + 1], dx - 1, dy - 1, dz - 1));
                    }
                    auto y1 = lerp(fy, x1, x2);
                    auto y2 = lerp(fy, x3, x4);

                    vectors.push_back((float) (lerp(fz, y1, y2) * amplitude));
                }
            }
        }

        return vectors;
    }
};

#endif
