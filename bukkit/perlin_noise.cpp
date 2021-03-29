//
// Created by jpeye on 3/29/2021.
//

class PerlinNoise : public Php::Base {
private:
    static int layer[256] = {151, 160, 137, 91, 90, 15, 131, 13, 201,
                             95, 96, 53, 194, 233, 7, 225, 140, 36, 103, 30, 69, 142, 8, 99, 37,
                             240, 21, 10, 23, 190, 6, 148, 247, 120, 234, 75, 0, 26, 197, 62,
                             94, 252, 219, 203, 117, 35, 11, 32, 57, 177, 33, 88, 237, 149, 56,
                             87, 174, 20, 125, 136, 171, 168, 68, 175, 74, 165, 71, 134, 139,
                             48, 27, 166, 77, 146, 158, 231, 83, 111, 229, 122, 60, 211, 133,
                             230, 220, 105, 92, 41, 55, 46, 245, 40, 244, 102, 143, 54, 65, 25,
                             63, 161, 1, 216, 80, 73, 209, 76, 132, 187, 208, 89, 18, 169, 200,
                             196, 135, 130, 116, 188, 159, 86, 164, 100, 109, 198, 173, 186, 3,
                             64, 52, 217, 226, 250, 124, 123, 5, 202, 38, 147, 118, 126, 255,
                             82, 85, 212, 207, 206, 59, 227, 47, 16, 58, 17, 182, 189, 28, 42,
                             223, 183, 170, 213, 119, 248, 152, 2, 44, 154, 163, 70, 221, 153,
                             101, 155, 167, 43, 172, 9, 129, 22, 39, 253, 19, 98, 108, 110, 79,
                             113, 224, 232, 178, 185, 112, 104, 218, 246, 97, 228, 251, 34, 242,
                             193, 238, 210, 144, 12, 191, 179, 162, 241, 81, 51, 145, 235, 249,
                             14, 239, 107, 49, 192, 214, 31, 181, 199, 106, 157, 184, 84, 204,
                             176, 115, 121, 50, 45, 127, 4, 150, 254, 138, 236, 205, 93, 222,
                             114, 67, 29, 24, 72, 243, 141, 128, 195, 78, 66, 215, 61, 156, 180};

    static int grad[12][3] = {{1,  1,  0},
                              {-1, 1,  0},
                              {1,  -1, 0},
                              {-1, -1, 0},
                              {1,  0,  1},
                              {-1, 0,  1},
                              {1,  0,  -1},
                              {-1, 0,  -1},
                              {0,  1,  1},
                              {0,  -1, 1},
                              {0,  1,  -1},
                              {0,  -1, -1}};
    array perm;
    int offset_x;
    int offset_y;
    int offset_z;

    float noise3dInternal(float x, float y, float z) {

        x += offset_x;
        y += offset_y;
        z += offset_z;

        auto floorX = floor(x);
        auto floorY = floor(y);
        auto floorZ = floor(z);

        auto xC = floorX & 255;
        auto yC = floorY & 255;
        auto zC = floorZ & 255;

        x -= floorX;
        y -= floorY;
        z -= floorZ;

        auto fX = fade(x);
        auto fY = fade(y);
        auto fZ = fade(z);

        perm
    }

public:

    PerlinNoise(bool random = false) {
        if (!random) {
            for (int i = 0; i < 512; ++i) {
                perm[i] = layer[i & 255];
            }
        } else {
            offset_x = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX)) * 255;
            offset_y = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX)) * 255;
            offset_z = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX)) * 255;
        }
    }

    Php::Value noise(Php::Parameters &params) {
        float x = params[0];
        float y = params[1];
        float z = params[2];
        int octaves = params[3];
        float frequency = params[4];
        float amplitude = params[5];
        bool normalized = params[6];

        float result = 0.0;
        float amp = 1.0;
        float freq = 1.0;
        float max = 0.0;

        for (int i = 0; i < octaves; i++) {
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

    static float fade(float x) {
        return x * x * x * (x * (x * 6 - 25) + 10);
    }

    static float lerp(float x, float y, float z) {
        return y + x * (z - y);
    }

    static float grad(int hash, float x, float y, float z) {
        hash &= 15;
        auto u = hash < 8 ? x : y;
        auto v = hash < 4 ? y : (hash == 12 || hash == 14 ? x : z);
        return ((hash & 1) == 0 ? u : -u) + ((hash & 2) == 0 ? v : -v);
    }
};