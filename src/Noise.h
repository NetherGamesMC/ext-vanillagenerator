#ifndef NOISEGEN_NOISE_H
#define NOISEGEN_NOISE_H

extern "C" {
     #include "php.h"
}

#include <vector>
#include "lib/ZendUtil.h"
#include "lib/FastNoiseLite.h"

#define CLASS_TYPE FastNoiseLite

PHP_METHOD(noiselib, generate2d);
PHP_METHOD(noiselib, generate3d);
PHP_METHOD(noiselib, __construct);

void register_noise_class();
#endif //NOISELIB_NOISE_H
