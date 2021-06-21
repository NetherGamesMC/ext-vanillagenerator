#ifndef EXT_NOISELIB_LIB_OBJECTS_BIOMEARRAY_H_
#define EXT_NOISELIB_LIB_OBJECTS_BIOMEARRAY_H_

#include <cstddef>
#include <gsl/span>

class BiomeArray {
 public:
  static const size_t DATA_SIZE = 256;

  explicit BiomeArray(const gsl::span<const uint_fast8_t, BiomeArray::DATA_SIZE> &values);

  auto get(uint8_t x, uint8_t z) const -> uint_fast8_t;
  auto set(uint8_t x, uint8_t z, uint_fast8_t value) -> void;

 private:
  std::array<uint_fast8_t, 256> mValues;

  static inline void index(uint8_t x, uint8_t z, unsigned int &offset);
};

#endif // EXT_NOISELIB_LIB_OBJECTS_BIOMEARRAY_H_
