#include <new>
#ifndef PERLIN_INCLUDED
#define PERLIN_INCLUDED

#include "Noise.h"
#include "FastNoiseLite.h"

#endif

#include <chrono>
#include <iostream>
#include <string>

typedef struct {
    FastNoiseLite fastNoiseLite;
    zend_object std;
} noise_object;

// private class reference
// ZEND_BEGIN_ARG_INFO(arginfo_noiseref, 0)
//     ZEND_ARG_INFO(1, fnoise)
// ZEND_END_ARG_INFO();

// generate2d {{
ZEND_BEGIN_ARG_INFO(arginfo_gen2d, 1)
    ZEND_ARG_INFO(0, x)
    ZEND_ARG_INFO(0, y)
    ZEND_ARG_INFO_WITH_DEFAULT_VALUE(0, x_off, 0)
    ZEND_ARG_INFO_WITH_DEFAULT_VALUE(0, z_off, 0)
ZEND_END_ARG_INFO();

PHP_METHOD(noiselib, generate2d) {
    auto object = fetch_from_zend_object<noise_object>(Z_OBJ_P(getThis()));
    auto noise = object->fastNoiseLite;

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
ZEND_BEGIN_ARG_INFO(arginfo_gen3d, 1)
    ZEND_ARG_INFO(0, x)
    ZEND_ARG_INFO(0, y)
    ZEND_ARG_INFO(0, z)
    ZEND_ARG_INFO_WITH_DEFAULT_VALUE(0, x_off, 0)
    ZEND_ARG_INFO_WITH_DEFAULT_VALUE(0, y_off, 0)
    ZEND_ARG_INFO_WITH_DEFAULT_VALUE(0, z_off, 0)
    ZEND_ARG_INFO_WITH_DEFAULT_VALUE(0, amplifier, 0)
ZEND_END_ARG_INFO();

PHP_METHOD(noiselib, generate3d) {
    auto object = fetch_from_zend_object<noise_object>(Z_OBJ_P(getThis()));
    auto noise = object->fastNoiseLite;

    zend_long x, y, z, x_off, y_off, z_off, amplifier;
    ZEND_PARSE_PARAMETERS_START(3, 7)
        Z_PARAM_LONG(x)
        Z_PARAM_LONG(y)
        Z_PARAM_LONG(z)
        Z_PARAM_LONG(x_off)
        Z_PARAM_LONG(y_off)
        Z_PARAM_LONG(z_off)
        Z_PARAM_LONG(amplifier)
    ZEND_PARSE_PARAMETERS_END();

    zend_array *data = zend_new_array(0);
    zend_ulong idx = 0;
    for (int xx = x_off; xx < (x + x_off); x++) {
        for (int yx = y_off; yx < (y + y_off); yx++) {
            for (int zx = z_off; zx < (z + z_off); zx++) {
                double nsx = (double) noise.GetNoise((float) xx, (float) zx);
                zval *val;
                ZVAL_DOUBLE(val, nsx);
                zend_hash_index_add(data, idx++, val);
            }
        }
    }
    RETVAL_ARR(data);
}
// }} __construct {{
ZEND_BEGIN_ARG_INFO(arginfo___construct, 1)
    ZEND_ARG_INFO_WITH_DEFAULT_VALUE(0, seed, 3306)
ZEND_END_ARG_INFO()

PHP_METHOD(PhpLightArray, __construct) {
    zend_long* seed;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
        Z_PARAM_STR(seed);
    ZEND_PARSE_PARAMETERS_END();

    auto object = fetch_from_zend_object<noise_object>(Z_OBJ_P(getThis()));
    new (&object->fastNoiseLite) FastNoiseLite(seed);
}
// }}

// Register methods to class
zend_function_entry noise_lib_methods[] = {
    PHP_ME(noiselib, generate2d, arginfo_gen2d, ZEND_ACC_PUBLIC)
    PHP_ME(noiselib, generate3d, arginfo_gen3d, ZEND_ACC_PUBLIC)
    PHP_FE_END
};

void register_noise_class() {
    zend_class_entry cle;
    INIT_CLASS_ENTRY(cle, "Noise", noise_lib_methods);
    zend_register_internal_class(&cle);
}