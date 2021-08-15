#ifndef EXT_VANILLAGENERATOR_LIB_MORTONHELPER_H_
#define EXT_VANILLAGENERATOR_LIB_MORTONHELPER_H_

extern "C" {
#include <php.h>
}

#include <morton.h>
#include <stdexcept>

#define MORTON3D_BIT_SIZE 21
#define BLOCKHASH_Y_BITS 9
#define BLOCKHASH_Y_PADDING 128
#define BLOCKHASH_Y_OFFSET BLOCKHASH_Y_PADDING
#define BLOCKHASH_Y_MASK ((1 << BLOCKHASH_Y_BITS) - 1)
#define BLOCKHASH_XZ_MASK ((1 << MORTON3D_BIT_SIZE) - 1)
#define BLOCKHASH_XZ_EXTRA_BITS 6
#define BLOCKHASH_XZ_EXTRA_MASK ((1 << BLOCKHASH_XZ_EXTRA_BITS) - 1)
#define BLOCKHASH_XZ_SIGN_SHIFT (64 - MORTON3D_BIT_SIZE - BLOCKHASH_XZ_EXTRA_BITS)
#define BLOCKHASH_X_SHIFT BLOCKHASH_Y_BITS
#define BLOCKHASH_Z_SHIFT (BLOCKHASH_X_SHIFT + BLOCKHASH_XZ_EXTRA_BITS)

static uint_fast64_t morton2d_encode(int64_t x, int64_t y) {
  return libmorton::morton2D_64_encode(x, y);
}

static void morton2d_decode(int64_t chunkCord, int64_t &chunkX, int64_t &chunkZ) {
  const size_t SHIFT = (sizeof(zend_long) * 8) - 32;

  uint_fast32_t x, y;

  libmorton::morton2D_64_decode(chunkCord, x, y);

  chunkX = static_cast<zend_long>(x) << SHIFT >> SHIFT;
  chunkZ = static_cast<zend_long>(y) << SHIFT >> SHIFT;
}

static uint_fast64_t morton3d_encode(int_fast64_t x, int_fast32_t y, int_fast64_t z) {
  int64_t shiftedY = y + BLOCKHASH_Y_OFFSET;
  if ((shiftedY & (~0 << BLOCKHASH_Y_BITS)) != 0) {
    throw std::invalid_argument("Y coordinate y is out of range!");
  }

  //morton3d gives us 21 bits on each axis, but the Y Axis only requires 9
  //so we use the extra space on Y (12 bits) and add 6 extra bits from X and Z instead.
  //if we ever need more space for Y (e.g. due to expansion), take bits from X/Z to compensate.
  return libmorton::morton3D_64_encode(x & BLOCKHASH_XZ_MASK, (shiftedY /* & BLOCKHASH_Y_MASK */)
      | (((x >> MORTON3D_BIT_SIZE) & BLOCKHASH_XZ_EXTRA_MASK) << BLOCKHASH_X_SHIFT)
      | (((z >> MORTON3D_BIT_SIZE) & BLOCKHASH_XZ_EXTRA_MASK) << BLOCKHASH_Z_SHIFT), z & BLOCKHASH_XZ_MASK);
}

static void morton3d_decode(uint_fast64_t chunkCord, int_fast32_t &rx, int_fast32_t &ry, int_fast32_t &rz) {
  const size_t SHIFT = (sizeof(zend_long) * 8) - 32;

  uint_fast32_t x, y, z;

  libmorton::morton3D_64_decode(chunkCord, x, y, z);

  int64_t baseX = static_cast<zend_long>(x) << SHIFT >> SHIFT;
  int64_t baseY = static_cast<zend_long>(y) << SHIFT >> SHIFT;
  int64_t baseZ = static_cast<zend_long>(z) << SHIFT >> SHIFT;

  int64_t extraX = (((baseY >> BLOCKHASH_X_SHIFT) & BLOCKHASH_XZ_EXTRA_MASK) << MORTON3D_BIT_SIZE);
  int64_t extraZ = (((baseY >> BLOCKHASH_Z_SHIFT) & BLOCKHASH_XZ_EXTRA_MASK) << MORTON3D_BIT_SIZE);

  rx = static_cast<int_fast32_t>(((baseX & BLOCKHASH_XZ_MASK) | extraX) << BLOCKHASH_XZ_SIGN_SHIFT >> BLOCKHASH_XZ_SIGN_SHIFT);
  ry = static_cast<int_fast32_t>((baseY & BLOCKHASH_Y_MASK) - BLOCKHASH_Y_OFFSET);
  rz = static_cast<int_fast32_t>(((baseZ & BLOCKHASH_XZ_MASK) | extraZ) << BLOCKHASH_XZ_SIGN_SHIFT >> BLOCKHASH_XZ_SIGN_SHIFT);
}

#endif // EXT_VANILLAGENERATOR_LIB_MORTONHELPER_H_
