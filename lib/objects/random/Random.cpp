#include <lib/objects/random/Random.h>

Random::Random(int_fast64_t iSeed): seed(iSeed) {
  SetSeed(seed);
}

void Random::SetSeed(int_fast64_t mSeed) {
  this->seed = mSeed;

  const int_fast64_t X = 123456789;
  const int_fast64_t Y = 362436069;
  const int_fast64_t Z = 521288629;
  const int_fast64_t W = 88675123;

  this->x = X ^ mSeed;
  this->y = Y ^ (mSeed << 17) | ((mSeed >> 15) & 0x7fffffff) & 0xffffffff;
  this->z = Z ^ (mSeed << 31) | ((mSeed >> 1) & 0x7fffffff) & 0xffffffff;
  this->w = W ^ (mSeed << 18) | ((mSeed >> 14) & 0x7fffffff) & 0xffffffff;
}

int_fast64_t Random::NextSignedInt() {
  int_fast64_t t = (x ^ (x << 11)) & 0xffffffff;

  x = y;
  y = z;
  z = w;
  w = (w ^ ((w >> 19) & 0x7fffffff) ^ (t ^ ((t >> 8) & 0x7fffffff))) & 0xffffffff;

  return w;
}

int_fast64_t Random::NextInt() {
  return NextSignedInt() & 0x7fffffff;
}

int_fast64_t Random::NextInt(int_fast64_t bound) {
  return NextInt() % bound;
}

float Random::NextFloat() {
  return (float) NextInt() / 2147483647.f;
}

float Random::NextSignedFloat() {
  return (float) NextSignedInt() / 2147483647.f;
}

bool Random::NextBoolean() {
  return (NextSignedInt() & 0x01) == 0;
}

int_fast64_t Random::NextLong() {
  return (NextSignedInt() << 32) | NextSignedInt();
}

int_fast64_t Random::NextRange(int_fast64_t start, int_fast64_t end) {
  return start + (NextInt() % (end + 1 - start));
}

int_fast64_t Random::GetSeed() const {
  return seed;
}