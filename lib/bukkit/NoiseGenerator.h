#ifndef BASE_NOISE
#define BASE_NOISE

#include <cstdint>

#endif

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

    static float lerp(float a, float b, float t) {
        return a + t * (b - a);
    }

    virtual float noise3d(float x, float y, float z) {
        return 0.0;
    }

    virtual void setPermutations(uint8_t permutations[256]) {
        for (auto i = 0; i < 256; i++) {
            perm[i] = permutations[i];
        }
    }

    void setOffset(float off_x, float off_y, float off_z) {

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

    uint8_t perm[256] = {
            151, 160, 137, 91, 90, 15,
            131, 13, 201, 95, 96, 53, 194, 233, 7, 225, 140, 36, 103, 30, 69, 142, 8, 99, 37, 240, 21, 10, 23,
            190, 6, 148, 247, 120, 234, 75, 0, 26, 197, 62, 94, 252, 219, 203, 117, 35, 11, 32, 57, 177, 33,
            88, 237, 149, 56, 87, 174, 20, 125, 136, 171, 168, 68, 175, 74, 165, 71, 134, 139, 48, 27, 166,
            77, 146, 158, 231, 83, 111, 229, 122, 60, 211, 133, 230, 220, 105, 92, 41, 55, 46, 245, 40, 244,
            102, 143, 54, 65, 25, 63, 161, 1, 216, 80, 73, 209, 76, 132, 187, 208, 89, 18, 169, 200, 196,
            135, 130, 116, 188, 159, 86, 164, 100, 109, 198, 173, 186, 3, 64, 52, 217, 226, 250, 124, 123,
            5, 202, 38, 147, 118, 126, 255, 82, 85, 212, 207, 206, 59, 227, 47, 16, 58, 17, 182, 189, 28, 42,
            223, 183, 170, 213, 119, 248, 152, 2, 44, 154, 163, 70, 221, 153, 101, 155, 167, 43, 172, 9,
            129, 22, 39, 253, 19, 98, 108, 110, 79, 113, 224, 232, 178, 185, 112, 104, 218, 246, 97, 228,
            251, 34, 242, 193, 238, 210, 144, 12, 191, 179, 162, 241, 81, 51, 145, 235, 249, 14, 239, 107,
            49, 192, 214, 31, 181, 199, 106, 157, 184, 84, 204, 176, 115, 121, 50, 45, 127, 4, 150, 254,
            138, 236, 205, 93, 222, 114, 67, 29, 24, 72, 243, 141, 128, 195, 78, 66, 215, 61, 156, 180
    };
};
