#ifndef EXT_NOISELIB_RANDOM_H
#define EXT_NOISELIB_RANDOM_H

#include <random>
#include <cstdint>

class Random {
public:
    const int64_t X = 123456789;
    const int64_t Y = 362436069;
    const int64_t Z = 521288629;
    const int64_t W = 88675123;

    Random() {
        setSeed(145237745); // TODO: Randomize this value
    }

    Random(int64_t seed) {
        setSeed(seed);
    }

    void setSeed(int64_t seed) {
        this->x = X ^ seed;
        this->y = Y ^ (seed << 17) | ((seed >> 15) & 0x7fffffff) & 0xffffffff;
        this->z = Z ^ (seed << 31) | ((seed >> 1) & 0x7fffffff) & 0xffffffff;
        this->w = W ^ (seed << 18) | ((seed >> 14) & 0x7fffffff) & 0xffffffff;
    }

    int64_t nextInt() {
        return nextSignedInt() & 0x7fffffff;
    }

    int64_t nextSignedInt() {
        int64_t t = (x ^ (x << 11)) & 0xffffffff;

        x = y;
        y = z;
        z = w;
        w = (w ^ ((w >> 19) & 0x7fffffff) ^ (t ^ ((t >> 8) & 0x7fffffff))) & 0xffffffff;

        return w;
    }

    /**
     * Returns a float between 0.0 and 1.0 (inclusive)
     */
    float nextFloat() {
        return (float) nextInt() / 2147483647.f;
    }

    /**
     * Returns a float between -1.0 and 1.0 (inclusive)
     */
    float nextSignedFloat() {
        return (float) nextSignedInt() / 2147483647.f;
    }

    /**
     * Returns a random boolean
     */
    bool nextBoolean() {
        return (nextSignedInt() & 0x01) == 0;
    }

    /**
     * Returns a random integer between start and end
     *
     * @param int start default 0
     * @param int end default 0x7fffffff
     */
    int64_t nextRange(int start = 0, int end = 0x7fffffff) {
        return start + (nextInt() % (end + 1 - start));
    }

    int64_t nextBoundedInt(int bound) {
        return nextInt() % bound;
    }

private:
    int64_t x;
    int64_t y;
    int64_t z;
    int64_t w;

    static std::mt19937_64 rng;
};

#endif

