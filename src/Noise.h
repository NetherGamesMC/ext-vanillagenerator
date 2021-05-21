#ifndef NOISEGEN_NOISE_H
#define NOISEGEN_NOISE_H

extern "C" {
     #include "php.h"
}

#include <vector>
#include "lib/ZendUtil.h"
#include "lib/FastNoiseLite.h"

#define CLASS_TYPE FastNoiseLite

PHP_METHOD(noiselib, __construct);
PHP_METHOD(noiselib, generate2d);
PHP_METHOD(noiselib, generate3d);

void register_noise_class();
#endif
