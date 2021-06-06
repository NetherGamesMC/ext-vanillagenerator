#ifndef EXT_NOISELIB_RANDOM_H
#define EXT_NOISELIB_RANDOM_H

#include <cstdint>

// Fun fact, PHP_INT_MAX value is 9,223,372,036,854,775,807, which equals to 2^63 − 1, or known as long long int (int64_t)
// Want some interesting facts? Random class in C++ alone took 254200ns/op while in PHP userland, it took 1876600ns/op
// to execute nextInt() function in a loop of 10000
class Random {
public:
    const int64_t X = 123456789;
    const int64_t Y = 362436069;
    const int64_t Z = 521288629;
    const int64_t W = 88675123;

    Random(int64_t seed) {
        setSeed(seed);
    }

    void setSeed(int64_t mSeed) {
        this->seed = mSeed;

        this->x = X ^ mSeed;
        this->y = Y ^ (mSeed << 17) | ((mSeed >> 15) & 0x7fffffff) & 0xffffffff;
        this->z = Z ^ (mSeed << 31) | ((mSeed >> 1) & 0x7fffffff) & 0xffffffff;
        this->w = W ^ (mSeed << 18) | ((mSeed >> 14) & 0x7fffffff) & 0xffffffff;
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

    int64_t nextLong() {
        return (nextSignedInt() << 32) | nextSignedInt();
    }

    /**
     * Returns a random integer between start and end
     *
     * @param int start default 0
     * @param int end default 0x7fffffff
     */
    int64_t nextRange(int64_t start = 0, int64_t end = 0x7fffffff) {
        return start + (nextInt() % (end + 1 - start));
    }

    int64_t nextBoundedInt(int64_t bound) {
        return nextInt() % bound;
    }

    int64_t getSeed() const {
        return seed;
    }

private:
    int64_t x;
    int64_t y;
    int64_t z;
    int64_t w;

    int64_t seed;
};

#endif

