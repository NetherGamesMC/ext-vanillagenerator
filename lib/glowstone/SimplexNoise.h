#ifndef SIMPLEX_NOISE_H
#define SIMPLEX_NOISE_H

#include <cstdint>
#include "PerlinNoise.h"

class SimplexNoise : public PerlinNoise {
public:
    float noise3d(float xin, float yin, float zin) override {
        if (yin == 0.0) {
            return PerlinNoise::noise3d(xin, yin, zin);
        }

        xin += offset_x;
        yin += offset_y;
        if (xin == 0.0) {
            return simplex2D(xin, yin);
        }

        zin += offset_z;
        return simplex3D(xin, yin, zin);
    }

protected:
    std::vector<float>
    get2dNoise(float x, float z, int size_x, int size_y, float scale_x, float scale_y, float amplitude) override {
        std::vector<float> noise;
        for (auto i = 0; i < size_y; ++i) {
            auto zin = offset_y + (z + (float) i) * scale_y;
            for (auto j = 0; j < size_x; ++j) {
                auto xin = offset_x + (x + (float) j) * scale_x;
                noise.push_back(simplex2D(xin, zin) * amplitude);
            }
        }

        return noise;
    }

    std::vector<float>
    get3dNoise(float x, float y, float z, int size_x, int size_y, int sizeZ, float scale_x, float scale_y,
               float scale_z, float amplitude) override {
        std::vector<float> noise;
        for (int i = 0; i < sizeZ; ++i) {
            auto zin = offset_z + (z + (float) i) * scale_z;
            for (int j = 0; j < size_x; ++j) {
                auto xin = offset_x + (x + (float) j) * scale_x;
                for (int k = 0; k < size_y; ++k) {
                    auto yin = offset_y + (y + (float) k) * scale_y;

                    noise.push_back(simplex3D(xin, yin, zin) * amplitude);
                }
            }
        }

        return noise;
    }

private:
    float simplex2D(float x, float y) {
        float n0, n1, n2;   // Noise contributions from the three corners

        // Skewing/Unskewing factors for 2D
        static const float F2 = 0.366025403f;  // F2 = (sqrt(3) - 1) / 2
        static const float G2 = 0.211324865f;  // G2 = (3 - sqrt(3)) / 6   = F2 / (1 + 2 * K)

        // Skew the input space to determine which simplex cell we're in
        const float s = (x + y) * F2;  // Hairy factor for 2D
        const float xs = x + s;
        const float ys = y + s;
        const int32_t i = fastfloor(xs);
        const int32_t j = fastfloor(ys);

        // Unskew the cell origin back to (xin,y) space
        const float t = static_cast<float>(i + j) * G2;
        const float X0 = i - t;
        const float Y0 = j - t;
        const float x0 = x - X0;  // The xin,y distances from the cell origin
        const float y0 = y - Y0;

        // For the 2D case, the simplex shape is an equilateral triangle.
        // Determine which simplex we are in.
        int32_t i1, j1;  // Offsets for second (middle) corner of simplex in (i,j) coords
        if (x0 > y0) {   // lower triangle, XY order: (0,0)->(1,0)->(1,1)
            i1 = 1;
            j1 = 0;
        } else {   // upper triangle, YX order: (0,0)->(0,1)->(1,1)
            i1 = 0;
            j1 = 1;
        }

        // A step of (1,0) in (i,j) means a step of (1-c,-c) in (xin,y), and
        // a step of (0,1) in (i,j) means a step of (-c,1-c) in (xin,y), where
        // c = (3-sqrt(3))/6

        const float x1 = x0 - i1 + G2;            // Offsets for middle corner in (xin,y) unskewed coords
        const float y1 = y0 - j1 + G2;
        const float x2 = x0 - 1.0f + 2.0f * G2;   // Offsets for last corner in (xin,y) unskewed coords
        const float y2 = y0 - 1.0f + 2.0f * G2;

        // Work out the hashed gradient indices of the three simplex corners
        const int gi0 = hash(i + hash(j));
        const int gi1 = hash(i + i1 + hash(j + j1));
        const int gi2 = hash(i + 1 + hash(j + 1));

        // Calculate the contribution from the first corner
        float t0 = 0.5f - x0 * x0 - y0 * y0;
        if (t0 < 0.0f) {
            n0 = 0.0f;
        } else {
            t0 *= t0;
            n0 = t0 * t0 * grad(gi0, x0, y0);
        }

        // Calculate the contribution from the second corner
        float t1 = 0.5f - x1 * x1 - y1 * y1;
        if (t1 < 0.0f) {
            n1 = 0.0f;
        } else {
            t1 *= t1;
            n1 = t1 * t1 * grad(gi1, x1, y1);
        }

        // Calculate the contribution from the third corner
        float t2 = 0.5f - x2 * x2 - y2 * y2;
        if (t2 < 0.0f) {
            n2 = 0.0f;
        } else {
            t2 *= t2;
            n2 = t2 * t2 * grad(gi2, x2, y2);
        }

        // Add contributions from each corner to get the final noise value.
        // The result is scaled to return values in the interval [-1,1].
        return 70.0f * (n0 + n1 + n2);
    }

    float simplex3D(float x, float y, float z) {
        float n0, n1, n2, n3; // Noise contributions from the four corners

        // Skewing/Unskewing factors for 3D
        static const float F3 = 1.0f / 3.0f;
        static const float G3 = 1.0f / 6.0f;

        // Skew the input space to determine which simplex cell we're in
        float s = (x + y + z) * F3; // Very nice and simple skew factor for 3D
        int i = fastfloor(x + s);
        int j = fastfloor(y + s);
        int k = fastfloor(z + s);
        float t = (i + j + k) * G3;
        float X0 = i - t; // Unskew the cell origin back to (xin,y,z) space
        float Y0 = j - t;
        float Z0 = k - t;
        float x0 = x - X0; // The xin,y,z distances from the cell origin
        float y0 = y - Y0;
        float z0 = z - Z0;

        // For the 3D case, the simplex shape is a slightly irregular tetrahedron.
        // Determine which simplex we are in.
        int i1, j1, k1; // Offsets for second corner of simplex in (i,j,k) coords
        int i2, j2, k2; // Offsets for third corner of simplex in (i,j,k) coords
        if (x0 >= y0) {
            if (y0 >= z0) {
                i1 = 1;
                j1 = 0;
                k1 = 0;
                i2 = 1;
                j2 = 1;
                k2 = 0; // X Y Z order
            } else if (x0 >= z0) {
                i1 = 1;
                j1 = 0;
                k1 = 0;
                i2 = 1;
                j2 = 0;
                k2 = 1; // X Z Y order
            } else {
                i1 = 0;
                j1 = 0;
                k1 = 1;
                i2 = 1;
                j2 = 0;
                k2 = 1; // Z X Y order
            }
        } else { // x0<y0
            if (y0 < z0) {
                i1 = 0;
                j1 = 0;
                k1 = 1;
                i2 = 0;
                j2 = 1;
                k2 = 1; // Z Y X order
            } else if (x0 < z0) {
                i1 = 0;
                j1 = 1;
                k1 = 0;
                i2 = 0;
                j2 = 1;
                k2 = 1; // Y Z X order
            } else {
                i1 = 0;
                j1 = 1;
                k1 = 0;
                i2 = 1;
                j2 = 1;
                k2 = 0; // Y X Z order
            }
        }

        // A step of (1,0,0) in (i,j,k) means a step of (1-c,-c,-c) in (xin,y,z),
        // a step of (0,1,0) in (i,j,k) means a step of (-c,1-c,-c) in (xin,y,z), and
        // a step of (0,0,1) in (i,j,k) means a step of (-c,-c,1-c) in (xin,y,z), where
        // c = 1/6.
        float x1 = x0 - i1 + G3; // Offsets for second corner in (xin,y,z) coords
        float y1 = y0 - j1 + G3;
        float z1 = z0 - k1 + G3;
        float x2 = x0 - i2 + 2.0f * G3; // Offsets for third corner in (xin,y,z) coords
        float y2 = y0 - j2 + 2.0f * G3;
        float z2 = z0 - k2 + 2.0f * G3;
        float x3 = x0 - 1.0f + 3.0f * G3; // Offsets for last corner in (xin,y,z) coords
        float y3 = y0 - 1.0f + 3.0f * G3;
        float z3 = z0 - 1.0f + 3.0f * G3;

        // Work out the hashed gradient indices of the four simplex corners
        int gi0 = hash(i + hash(j + hash(k)));
        int gi1 = hash(i + i1 + hash(j + j1 + hash(k + k1)));
        int gi2 = hash(i + i2 + hash(j + j2 + hash(k + k2)));
        int gi3 = hash(i + 1 + hash(j + 1 + hash(k + 1)));

        // Calculate the contribution from the four corners
        float t0 = 0.6f - x0 * x0 - y0 * y0 - z0 * z0;
        if (t0 < 0) {
            n0 = 0.0;
        } else {
            t0 *= t0;
            n0 = t0 * t0 * grad(gi0, x0, y0, z0);
        }
        float t1 = 0.6f - x1 * x1 - y1 * y1 - z1 * z1;
        if (t1 < 0) {
            n1 = 0.0;
        } else {
            t1 *= t1;
            n1 = t1 * t1 * grad(gi1, x1, y1, z1);
        }
        float t2 = 0.6f - x2 * x2 - y2 * y2 - z2 * z2;
        if (t2 < 0) {
            n2 = 0.0;
        } else {
            t2 *= t2;
            n2 = t2 * t2 * grad(gi2, x2, y2, z2);
        }
        float t3 = 0.6f - x3 * x3 - y3 * y3 - z3 * z3;
        if (t3 < 0) {
            n3 = 0.0;
        } else {
            t3 *= t3;
            n3 = t3 * t3 * grad(gi3, x3, y3, z3);
        }
        // Add contributions from each corner to get the final noise value.
        // The result is scaled to stay just inside [-1,1]
        return 32.0f * (n0 + n1 + n2 + n3);
    }
};

#endif
