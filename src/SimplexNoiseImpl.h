#ifndef SIMPLEX_NOISE_IMPL_H
#define SIMPLEX_NOISE_IMPL_H

extern "C" {
#include "php.h"
#include "ext/spl/spl_exceptions.h"
#include "ext/standard/php_var.h"
#include "Zend/zend_exceptions.h"
}

#include <vector>
#include "lib/ZendUtil.h"
#include "lib/glowstone/SimplexNoise.h"
#include "lib/pocketmine/Random.h"

PHP_METHOD(simplex, __construct);
PHP_METHOD(simplex, getNoise);

void register_simplex_noise();

#endif //SIMPLEX_NOISE_IMPL_H
