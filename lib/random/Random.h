#ifndef EXT_NOISELIB_RANDOM_H
#define EXT_NOISELIB_RANDOM_H

#include <cstdint>

// Fun fact, PHP_INT_MAX value is 9,223,372,036,854,775,807, which equals to 2^63 − 1, or known as long long int (int64_t)
// Want some interesting facts? Random class in C++ alone took 254200ns/op while in PHP userland, it took 1876600ns/op
// to execute nextInt() function in a loop of 10000
class Random {
 public:
  const int_fast64_t X = 123456789;
  const int_fast64_t Y = 362436069;
  const int_fast64_t Z = 521288629;
  const int_fast64_t W = 88675123;

  Random(int_fast64_t seed);

  void setSeed(int_fast64_t mSeed);

  auto nextInt() -> int_fast64_t;
  auto nextSignedInt() -> int_fast64_t;

  /**
   * Returns a float between 0.0 and 1.0 (inclusive)
   */
  auto nextFloat() -> float;

  /**
   * Returns a float between -1.0 and 1.0 (inclusive)
   */
  auto nextSignedFloat() -> float;

  /**
   * Returns a random boolean
   */
  auto nextBoolean() -> bool;

  auto nextLong() -> int_fast64_t;

  /**
   * Returns a random integer between start and end
   *
   * @param int start default 0
   * @param int end default 0x7fffffff
   */
  auto nextRange(int_fast64_t start = 0, int_fast64_t end = 0x7fffffff) -> int_fast64_t;

  auto nextBoundedInt(int_fast64_t bound) -> int_fast64_t;
  auto getSeed() const -> int_fast64_t;

 private:
  int_fast64_t x;
  int_fast64_t y;
  int_fast64_t z;
  int_fast64_t w;

  int_fast64_t seed;
};

#endif

