#include <php.h>
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

// generate2d {{
ZEND_BEGIN_ARG_INFO(arginfo_gen2d, 1)
    ZEND_ARG_INFO(1, x)
    ZEND_ARG_INFO(1, y)
    ZEND_ARG_INFO_WITH_DEFAULT_VALUE(1, x_off, 0)
    ZEND_ARG_INFO_WITH_DEFAULT_VALUE(1, z_off, 0)
ZEND_END_ARG_INFO();

PHP_METHOD(noiselib, generate2d) {
    zend_long x, z, x_off, z_off;
    ZEND_PARSE_PARAMETERS_START(2, 4)
        Z_PARAM_LONG(x)
        Z_PARAM_LONG(z)
        Z_PARAM_LONG(x_off)
        Z_PARAM_LONG(z_off)
    ZEND_PARSE_PARAMETERS_END();

    zend_array *data = zend_new_array(0);
    zend_ulong idx = 0;
    for (int xx = x_off; xx < (x + x_off); x++) {
        for (int zx = z_off; zx < (z + z_off); zx++) {
            double nsx = (double) noise.GetNoise((float) xx, (float) zx);
            zval *val;
            ZVAL_DOUBLE(val, nsx);
            zend_hash_index_add(data, idx++, val);
        }
    }
    RETVAL_ARR(data);
}
// }} generate3d {{
// }}

// Register methods to class
static zend_function_entry noise_lib_methods[] = {
    PHP_ME(noiselib, generate2d, arginfo_gen2d, ZEND_ACC_PUBLIC)
    PHP_FE_END
};


void register_noise_class() {
    zend_class_entry cle;
    INIT_CLASS_ENTRY(cle, "Noise", noise_lib_methods);
    NoiseLib = zend_register_internal_class(&cle);
}