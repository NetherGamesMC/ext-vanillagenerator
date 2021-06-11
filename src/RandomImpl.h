#ifndef EXT_NOISELIB_RANDOMIMPL_H
#define EXT_NOISELIB_RANDOMIMPL_H

extern "C" {
#include "php.h"
#include "ext/date/php_date.h"
}

#include <new>
#include <lib/ZendUtil.h>
#include <lib/pocketmine/Random.h>

typedef struct {
    Random random;
    zend_object std;
} random_obj;

PHP_METHOD(Random, __construct);
PHP_METHOD(Random, setSeed);
PHP_METHOD(Random, getSeed);
PHP_METHOD(Random, nextInt);
PHP_METHOD(Random, nextSignedInt);
PHP_METHOD(Random, nextFloat);
PHP_METHOD(Random, nextSignedFloat);
PHP_METHOD(Random, nextBoolean);
PHP_METHOD(Random, nextLong);
PHP_METHOD(Random, nextRange);
PHP_METHOD(Random, nextBoundedInt);

extern zend_class_entry *random_entry;

void register_internal_randoms();

#endif //EXT_NOISELIB_RANDOMIMPL_H
