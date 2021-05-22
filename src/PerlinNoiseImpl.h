#ifndef PERLIN_NOISE_IMPL_H
#define PERLIN_NOISE_IMPL_H

extern "C" {
     #include "php.h"
     #include "ext/spl/spl_exceptions.h"
     #include "ext/standard/php_var.h"
     #include "Zend/zend_exceptions.h"
}

#include <vector>
#include "lib/ZendUtil.h"
#include "lib/glowstone/PerlinNoise.h"

#define CLASS_TYPE PerlinNoise

PHP_METHOD(perlin, __construct);
PHP_METHOD(perlin, getNoise);

void register_noise_class();

#endif
