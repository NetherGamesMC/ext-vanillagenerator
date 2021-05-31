#ifndef SIMPLEX_OCTAVE_GENERATOR_IMPL_H
#define SIMPLEX_OCTAVE_GENERATOR_IMPL_H


extern "C" {
#include "php.h"
#include "ext/spl/spl_exceptions.h"
#include "ext/standard/php_var.h"
#include "Zend/zend_exceptions.h"
}

#include <lib/ZendUtil.h>
#include <lib/vanilla/VanillaNoise.h>

PHP_METHOD(SimplexOctaveGenerator, fromRandomAndOctaves);
PHP_METHOD(SimplexOctaveGenerator, setScale);
PHP_METHOD(SimplexOctaveGenerator, getSizeX);
PHP_METHOD(SimplexOctaveGenerator, getSizeZ);
PHP_METHOD(SimplexOctaveGenerator, getFractalBrownianMotion);
PHP_METHOD(SimplexOctaveGenerator, noise);

void register_simplex_generator();

#endif
