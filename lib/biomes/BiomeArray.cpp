#include "BiomeArray.h"
#include <cstring>

BiomeArray::BiomeArray(const gsl::span<const uint8_t, 256> &values) {
  memcpy(mValues.data(), values.data(), sizeof(mValues));
}

uint8_t BiomeArray::get(const uint8_t x, const uint8_t z) const {
  uint_fast8_t offset;
  index(x, z, offset);

  return mValues[offset];
}

void BiomeArray::set(const uint8_t x, const uint8_t z, const uint8_t value) {
  uint_fast8_t offset;
  index(x, z, offset);

  mValues[offset] = value;
}

void BiomeArray::index(const uint8_t x, const uint8_t z, uint_fast8_t &offset) {
  offset = (z << 4) | x;
}

gsl::span<const uint_fast8_t, BiomeArray::DATA_SIZE> BiomeArray::getRawData() const {
  return gsl::span<const uint_fast8_t>(mValues);
}