#ifndef EXT_NOISE_MORTON_HELPER_H
#define EXT_NOISE_MORTON_HELPER_H

extern "C" {
#include <php.h>
}

#include <morton.h>

static uint_fast64_t morton2d_encode(int64_t x, int64_t y) {
    return libmorton::morton2D_64_encode(x, y);
}

static void morton2d_decode(int64_t chunkCord, int64_t &chunkX, int64_t &chunkZ){
    const size_t SHIFT = (sizeof(zend_long) * 8) - 32;

    uint_fast32_t x, y;

    libmorton::morton2D_64_decode(chunkCord, x, y);

    chunkX = static_cast<zend_long>(x) << SHIFT >> SHIFT;
    chunkZ = static_cast<zend_long>(y) << SHIFT >> SHIFT;
}

#endif //EXT_NOISE_MORTON_HELPER_H
