#ifndef PHP_CPP_INCL
#define PHP_CPP_INCL

#include <php.h>

#endif

#ifndef PERLIN_INCLUDED
#define PERLIN_INCLUDED

#include "FastNoiseLite.h"

#endif

#include <chrono>
#include <iostream>
#include <string>

zend_class_entry *NoiseLib;

// private class reference
// ZEND_BEGIN_ARG_INFO(arginfo_noiseref, 0)
//     ZEND_ARG_INFO(1, fnoise)
// ZEND_END_ARG_INFO();
FastNoiseLite noise = FastNoiseLite();

// generate2d
PHP_METHOD(noiselib, generate2d) {
    zend_long x, z, x_off, z_off;
    ZEND_PARSE_PARAMETERS_START(ZEND_PARSE_PARAMS_THROW, 3)
        Z_PARAM_LONG(x)
        Z_PARAM_LONG(z)
        Z_PARAM_LONG(x_off)
        Z_PARAM_LONG(z_off)
    ZEND_PARSE_PARAMETERS_END();

    zend_array *data = zend_new_array(0);
    zend_ulong idx = 0;
    for (int xx = x_off; xx < (x + x_off); x++) {
        for (int zx = z_off; zx < (z + z_off); zx++) {
            zend_hash_index_add(data, idx++, noise.GetNoise((float) xx, (float) zx));
            // how to access zend array ?
        }
    }
    RETVAL_ARR(data);
}
static zend_function_entry noise_lib_methods[] = {
    PHP_ME(noiselib, generate2d, ZEND_ACC_PUBLIC),
    PHP_FE_END
};


void register_noise_class() {
    zend_class_entry cle;
    INIT_CLASS_ENTRY(cle, "Noise", noise_lib_methods);
    NoiseLib = zend_register_internal_class(&cle);
}