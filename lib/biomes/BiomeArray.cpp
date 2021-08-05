#include "BiomeArray.h"
#include <cstring>

BiomeArray::BiomeArray(const gsl::span<const uint8_t, 256> &values) {
  memcpy(mValues.data(), values.data(), sizeof(mValues));
}

uint8_t BiomeArray::Get(const uint8_t x, const uint8_t z) const {
  uint_fast8_t offset;
  Index(x, z, offset);

  return mValues[offset];
}

void BiomeArray::Set(const uint8_t x, const uint8_t z, const uint8_t value) {
  uint_fast8_t offset;
  Index(x, z, offset);

  mValues[offset] = value;
}

void BiomeArray::Index(const uint8_t x, const uint8_t z, uint_fast8_t &offset) {
  offset = (z << 4) | x;
}

gsl::span<const uint_fast8_t, BiomeArray::DATA_SIZE> BiomeArray::GetRawData() const {
  return gsl::span<const uint_fast8_t>(mValues);
}