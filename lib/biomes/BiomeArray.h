#ifndef EXT_VANILLAGENERATOR_LIB_OBJECTS_BIOMEARRAY_H_
#define EXT_VANILLAGENERATOR_LIB_OBJECTS_BIOMEARRAY_H_

#include <cstddef>
#include <gsl/span>

class BiomeArray {
 public:
  static const size_t DATA_SIZE = 256;

  explicit BiomeArray(const gsl::span<const uint_fast8_t, BiomeArray::DATA_SIZE> &values);

  auto Get(uint8_t x, uint8_t z) const -> uint_fast8_t;
  auto Set(uint8_t x, uint8_t z, uint_fast8_t value) -> void;

  [[nodiscard]] gsl::span<const uint_fast8_t, DATA_SIZE> GetRawData() const;

 private:
  std::array<uint_fast8_t, 256> mValues;

  static inline void Index(uint8_t x, uint8_t z, uint_fast8_t &offset);
};

#endif // EXT_VANILLAGENERATOR_LIB_OBJECTS_BIOMEARRAY_H_
