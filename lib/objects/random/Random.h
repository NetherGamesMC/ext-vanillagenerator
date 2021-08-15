#ifndef EXT_VANILLAGENERATOR_RANDOM_H
#define EXT_VANILLAGENERATOR_RANDOM_H

#include <cstdint>

// Fun fact, PHP_INT_MAX value is 9,223,372,036,854,775,807, which equals to 2^63 − 1, or known as long long int (int64_t)
// Want some interesting facts? Random class in C++ alone took 254200ns/op while in PHP userland, it took 1876600ns/op
// to execute nextInt() function in a loop of 10000
class Random {
 public:
  Random(int_fast64_t seed);

  void SetSeed(int_fast64_t mSeed);

  auto NextInt() -> int_fast64_t;
  auto NextInt(int_fast64_t bound) -> int_fast64_t;

  auto NextSignedInt() -> int_fast64_t;

  /**
   * Returns a float between 0.0 and 1.0 (inclusive)
   */
  auto NextFloat() -> float;

  /**
   * Returns a float between -1.0 and 1.0 (inclusive)
   */
  auto NextSignedFloat() -> float;

  /**
   * Returns a random boolean
   */
  auto NextBoolean() -> bool;

  auto NextLong() -> int_fast64_t;

  /**
   * Returns a random integer between start and end
   *
   * @param int start default 0
   * @param int end default 0x7fffffff
   */
  auto NextRange(int_fast64_t start = 0, int_fast64_t end = 0x7fffffff) -> int_fast64_t;

  [[nodiscard]] auto GetSeed() const -> int_fast64_t;

 private:
  int_fast64_t x;
  int_fast64_t y;
  int_fast64_t z;
  int_fast64_t w;

  int_fast64_t seed;
};

#endif

