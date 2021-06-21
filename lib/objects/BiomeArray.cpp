#include "BiomeArray.h"

BiomeArray::BiomeArray(const gsl::span<const uint_fast8_t, 256> &values) {
  memcpy(mValues.data(), values.data(), sizeof(mValues));
}

uint_fast8_t BiomeArray::get(const uint8_t x, const uint8_t z) const {
  unsigned int offset;
  index(x, z, offset);

  return mValues[offset];
}

void BiomeArray::set(const uint8_t x, const uint8_t z, const uint_fast8_t value) {
  unsigned int offset;
  index(x, z, offset);

  mValues[offset] = value;
}

void BiomeArray::index(const uint8_t x, const uint8_t z, unsigned int &offset) {
  offset = (z << 4) | x;
}