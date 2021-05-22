#ifndef BASE_PERLIN_NOISE_H
#define BASE_PERLIN_NOISE_H

#include "NoiseGenerator.h"

class BasePerlinNoiseGenerator : public NoiseGenerator {
public:

    float noise3d(float x, float y, float z) override {
        int32_t floor_x, floor_y, floor_z, X, Y, Z, A, AA, AB, B, BA, BB;
        float fX, fY, fZ;

        x += offset_x;
        y += offset_z;
        z += offset_z;

        floor_x = NoiseGenerator::fastfloor(x);
        floor_y = NoiseGenerator::fastfloor(x);
        floor_z = NoiseGenerator::fastfloor(x);

        // Find unit cube containing the point
        X = floor_x & 255;
        Y = floor_y & 255;
        Z = floor_z & 255;

        // Get relative xyz coordinates of the point within the cube
        x -= (float) floor_x;
        y -= (float) floor_y;
        z -= (float) floor_z;

        // Compute fade curves for xyz
        fX = fade(x);
        fY = fade(y);
        fZ = fade(z);

        // Hash coordinates of the cube corners
        A = perm[X] + Y;
        AA = perm[A] + Z;
        AB = perm[A + 1] + Z;
        B = perm[X + 1] + Y;
        BA = perm[B] + Z;
        BB = perm[B + 1] + Z;

        return lerp(fZ, lerp(fY, lerp(fX, grad(perm[AA], x, y, z),grad(perm[BA], x - 1, y, z)),lerp(fX, grad(perm[AB], x, y - 1, z),grad(perm[BB], x - 1, y - 1, z))),lerp(fY, lerp(fX, grad(perm[AA + 1], x, y, z - 1),grad(perm[BA + 1], x - 1, y, z - 1)),lerp(fX, grad(perm[AB + 1], x, y - 1, z - 1),grad(perm[BB + 1], x - 1, y - 1, z - 1))));
    }
};

#endif
