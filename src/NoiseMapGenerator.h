#ifndef EXTNOISE_NOISE_MAP_GENERATOR
#define EXTNOISE_NOISE_MAP_GENERATOR

extern "C" {
#include "php.h"
}

#include "RandomImpl.h"
#include <lib/ZendUtil.h>
#include <lib/vanilla/VanillaNoise.h>

typedef struct {
    BukkitSimplexOctaveGenerator simplexOctave;
    zend_object std;
} noise_map_obj;

void register_noise_map_generator();

#endif //EXTNOISE_NOISE_MAP_GENERATOR
