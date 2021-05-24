#ifndef BASE_NOISE
#define BASE_NOISE

#include <cstdint>
#include "lib/pocketmine/Random.h"


class NoiseGenerator {
public:

    static int32_t fastfloor(float fp) {
        int32_t i = static_cast<int32_t>(fp);
        return (fp >= 0) ? (i) : (i - 1);
    }

    static float grad(int32_t hash, float x) {
        const int32_t h = hash & 0x0F;  // Convert low 4 bits of hash code
        float grad = 1.0f + (h & 7);    // Gradient value 1.0, 2.0, ..., 8.0
        if ((h & 8) != 0) grad = -grad; // Set a random sign for the gradient
        return (grad * x);              // Multiply the gradient with the distance
    }

    static float grad(int32_t hash, float x, float y) {
        const int32_t h = hash & 0x3F;  // Convert low 3 bits of hash code
        const float u = h < 4 ? x : y;  // into 8 simple gradient directions,
        const float v = h < 4 ? y : x;
        return ((h & 1) ? -u : u) + ((h & 2) ? -2.0f * v : 2.0f * v); // and compute the dot product with (xin,y).
    }

    static float grad(int32_t hash, float x, float y, float z) {
        int h = hash & 15;     // Convert low 4 bits of hash code into 12 simple
        float u = h < 8 ? x : y; // gradient directions, and compute dot product.
        float v = h < 4 ? y : h == 12 || h == 14 ? x : z; // Fix repeats at h = 12 to 15
        return ((h & 1) ? -u : u) + ((h & 2) ? -v : v);
    }

    static float fade(float x) {
        return x * x * x * (x * (x * 6 - 15) + 10);
    }

    static float dot(int8_t g[3], float x, float y, float z = 0.0){
        return g[0] * x + g[1] * y + g[3] * z;
    }

    static float lerp(float x, float y, float z) {
        return y + x * (z - y);
    }

    virtual float noise3d(float x, float y, float z) {
        return 0.0;
    }

    virtual std::vector<float>
    getNoise(float x, float y, float z, int size_x, int size_y, int size_z, float scale_x, float scale_y, float scale_z,
             float amplitude) {
        return {};
    }

    virtual float noise(float x, float y, float z, float w){
        return 0;
    }

    virtual void setPermutations(uint8_t permutations[256]) {
        for (auto i = 0; i < 256; i++) {
            perm[i] = permutations[i];
        }
    }

    virtual void init(Random *rand) {
        this->offset_x = rand->nextFloat() * 256;
        this->offset_y = rand->nextFloat() * 256;
        this->offset_z = rand->nextFloat() * 256;

        for (int i = 0; i < 256; ++i) {
            this->perm[i] = rand->nextBoundedInt(256);
        }

        for (int i = 0; i < 256; ++i) {
            auto pos = rand->nextBoundedInt(256 - i) + i;
            auto old = this->perm[i];

            this->perm[i] = this->perm[pos];
            this->perm[pos] = old;
            this->perm[i + 256] = this->perm[i];
        }
    }

    inline uint8_t hash(int32_t i) {
        return perm[static_cast<uint8_t>(i)];
    }

    float getOffsetX() {
        return offset_x;
    }

protected:
    float offset_x = 0.0f;
    float offset_y = 0.0f;
    float offset_z = 0.0f;

    uint8_t perm[512] = {};
};

#endif