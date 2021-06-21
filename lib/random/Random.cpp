#include <lib/random/Random.h>

Random::Random(int_fast64_t seed) {
  setSeed(seed);
}

void Random::setSeed(int_fast64_t mSeed) {
  this->seed = mSeed;

  this->x = X ^ mSeed;
  this->y = Y ^ (mSeed << 17) | ((mSeed >> 15) & 0x7fffffff) & 0xffffffff;
  this->z = Z ^ (mSeed << 31) | ((mSeed >> 1) & 0x7fffffff) & 0xffffffff;
  this->w = W ^ (mSeed << 18) | ((mSeed >> 14) & 0x7fffffff) & 0xffffffff;
}

int_fast64_t Random::nextSignedInt() {
  int_fast64_t t = (x ^ (x << 11)) & 0xffffffff;

  x = y;
  y = z;
  z = w;
  w = (w ^ ((w >> 19) & 0x7fffffff) ^ (t ^ ((t >> 8) & 0x7fffffff))) & 0xffffffff;

  return w;
}

int_fast64_t Random::nextInt() {
  return nextSignedInt() & 0x7fffffff;
}

float Random::nextFloat() {
  return (float) nextInt() / 2147483647.f;
}

float Random::nextSignedFloat() {
  return (float) nextSignedInt() / 2147483647.f;
}

bool Random::nextBoolean() {
  return (nextSignedInt() & 0x01) == 0;
}

int_fast64_t Random::nextLong() {
  return (nextSignedInt() << 32) | nextSignedInt();
}

int_fast64_t Random::nextRange(int_fast64_t start, int_fast64_t end) {
  return start + (nextInt() % (end + 1 - start));
}

int_fast64_t Random::nextBoundedInt(int_fast64_t bound) {
  return nextInt() % bound;
}

int_fast64_t Random::getSeed() const {
  return seed;
}