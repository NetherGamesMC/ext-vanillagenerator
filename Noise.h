#ifndef NOISEGEN_NOISE_H
#define NOISEGEN_NOISE_H

extern "C" {
     #include "php.h"
}

PHP_METHOD(noiselib, generate2d);
PHP_METHOD(noiselib, generate3d);

void register_noise_class();
#endif //NOISELIB_NOISE_H
