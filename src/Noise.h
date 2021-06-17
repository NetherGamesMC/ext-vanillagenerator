#ifndef EXT_NOISELIB_NOISE_H
#define EXT_NOISELIB_NOISE_H

extern "C" {
#include <php.h>
}

#include "RandomImpl.h"
#include <lib/ZendUtil.h>
#include <lib/vanilla/VanillaNoise.h>

typedef struct {
    BukkitSimplexOctaveGenerator bukkitSimplexOctave;
    SimplexOctaveGenerator simplexOctave;
    PerlinOctaveGenerator perlinOctave;

    zend_object std;
} noise_map_obj;

PHP_METHOD(SimplexOctaveGenerator, fromRandomAndOctaves);
PHP_METHOD(SimplexOctaveGenerator, setScale);
PHP_METHOD(SimplexOctaveGenerator, getSizeX);
PHP_METHOD(SimplexOctaveGenerator, getSizeZ);
PHP_METHOD(SimplexOctaveGenerator, getFractalBrownianMotion);
PHP_METHOD(SimplexOctaveGenerator, noise);

PHP_METHOD(PerlinOctaveGenerator, fromRandomAndOctaves);
PHP_METHOD(PerlinOctaveGenerator, setXScale);
PHP_METHOD(PerlinOctaveGenerator, setYScale);
PHP_METHOD(PerlinOctaveGenerator, setZScale);
PHP_METHOD(PerlinOctaveGenerator, getFractalBrownianMotion);
PHP_METHOD(PerlinOctaveGenerator, getSizeY);

PHP_METHOD(NoiseMapGenerator, fromRandomAndOctaves);
PHP_METHOD(NoiseMapGenerator, noise);

void register_noise_function();

#endif //EXT_NOISELIB_NOISE_H
