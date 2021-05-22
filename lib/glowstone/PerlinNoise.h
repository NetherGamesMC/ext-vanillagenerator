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
    std::vector<float>
    getNoise(float x, float y, float z, int size_x, int size_y, int size_z, float scale_x, float scale_y, float scale_z,
             float amplitude) {

        if (size_y == 1) {
            return PerlinNoise::get2dNoise(x, z, size_x, size_z, scale_x, scale_z, amplitude);
        }

        return PerlinNoise::get3dNoise(x, y, z, size_x, size_y, size_z, scale_x, scale_y, scale_z, amplitude);
    }

protected:
    static int32_t fastfloor(float fp) {
        int32_t i = static_cast<int32_t>(fp);
        return fp < i ? i - 1 : i;
    }

    virtual std::vector<float>
    get2dNoise(float x, float z, int size_x, int size_z, float scale_x, float scale_z, float amplitude) {
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

private:
    static float lerp(float a, float b, float t) {
        return a + t * (b - a);
    }
};

#endif
