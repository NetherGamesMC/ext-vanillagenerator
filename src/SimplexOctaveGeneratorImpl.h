#ifndef SIMPLEX_OCTAVE_GENERATOR_IMPL_H
#define SIMPLEX_OCTAVE_GENERATOR_IMPL_H

extern "C" {
#include "php.h"
}

#include "RandomImpl.h"
#include <lib/ZendUtil.h>
#include <lib/vanilla/VanillaNoise.h>

typedef struct {
    SimplexOctaveGenerator simplexOctave;
    zend_object std;
} simplex_octave_obj;

PHP_METHOD(SimplexOctaveGenerator, fromRandomAndOctaves);
PHP_METHOD(SimplexOctaveGenerator, setScale);
PHP_METHOD(SimplexOctaveGenerator, getSizeX);
PHP_METHOD(SimplexOctaveGenerator, getSizeZ);
PHP_METHOD(SimplexOctaveGenerator, getFractalBrownianMotion);
PHP_METHOD(SimplexOctaveGenerator, noise);

void register_simplex_generator();

#endif
