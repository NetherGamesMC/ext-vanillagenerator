#ifndef PERLIN_OCTAVE_GENERATOR_IMPL_H
#define PERLIN_OCTAVE_GENERATOR_IMPL_H

extern "C" {
#include "php.h"
}

#include "RandomImpl.h"
#include <lib/ZendUtil.h>
#include <lib/vanilla/VanillaNoise.h>

typedef struct {
    PerlinOctaveGenerator perlinOctave;
    zend_object std;
} perlin_octave_obj;

PHP_METHOD(SimplexOctaveGenerator, fromRandomAndOctaves);
PHP_METHOD(SimplexOctaveGenerator, setXScale);
PHP_METHOD(SimplexOctaveGenerator, setYScale);
PHP_METHOD(SimplexOctaveGenerator, setZScale);
PHP_METHOD(SimplexOctaveGenerator, getFractalBrownianMotion);
PHP_METHOD(SimplexOctaveGenerator, getSizeY);

void register_perlin_generator();

#endif
