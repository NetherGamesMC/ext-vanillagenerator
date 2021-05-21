#ifndef PERLIN_INCLUDED
#define PERLIN_INCLUDED

#include "Noise.h"

#endif

// __construct {{

ZEND_BEGIN_ARG_INFO_EX(arginfo___construct, 0, 0, 1)
    ZEND_ARG_TYPE_INFO(0, seed, IS_LONG, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(noiselib, __construct) {
    zend_long seed;

    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_LONG(seed)
    ZEND_PARSE_PARAMETERS_END();

    SET_PHP_OBJECT_CONTAINER(new FastNoiseLite((int)seed));
}

// }} generate2d {{

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_gen2d, 0, 2, IS_ARRAY, 0)
    ZEND_ARG_TYPE_INFO(0, x, IS_LONG, 0)
    ZEND_ARG_TYPE_INFO(0, y, IS_LONG, 0)
    ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, x_off, IS_LONG, 0, nullptr)
    ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, z_off, IS_LONG, 0, nullptr)
ZEND_END_ARG_INFO();

PHP_METHOD(noiselib, generate2d) {
    FETCH_PHP_OBJECT_CONTAINER_VAR(container);

    zend_long x, z, x_off = 0, z_off = 0;
    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 2, 4)
        Z_PARAM_LONG(x)
        Z_PARAM_LONG(z)
        Z_PARAM_OPTIONAL
        Z_PARAM_LONG(x_off)
        Z_PARAM_LONG(z_off)
    ZEND_PARSE_PARAMETERS_END();

    std::vector<double> noises;
    for (int xx = x_off; xx < (x + x_off); xx++) {
        for (int zx = z_off; zx < (z + z_off); zx++) {
            noises.push_back((double)container->GetNoise((float) xx, (float) zx));
        }
    }

    zval *val;
    zend_array *array = zend_new_array(noises.size());
    for (int i = 0; i < noises.size(); ++i) {
        ZVAL_DOUBLE(val, noises[i]);

        zend_hash_index_add(array, i, val);
    }

    RETURN_ARR(array);
}

// }} generate3d {{

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_gen3d, 0, 3, IS_ARRAY, 0)
    ZEND_ARG_INFO(0, x)
    ZEND_ARG_INFO(0, y)
    ZEND_ARG_INFO(0, z)
    ZEND_ARG_INFO_WITH_DEFAULT_VALUE(0, x_off, nullptr)
    ZEND_ARG_INFO_WITH_DEFAULT_VALUE(0, y_off, nullptr)
    ZEND_ARG_INFO_WITH_DEFAULT_VALUE(0, z_off, nullptr)
ZEND_END_ARG_INFO();

PHP_METHOD(noiselib, generate3d) {
    FETCH_PHP_OBJECT_CONTAINER_VAR(container);

    zend_long x, y, z, x_off = 0, y_off = 0, z_off = 0;
    ZEND_PARSE_PARAMETERS_START(3, 7)
        Z_PARAM_LONG(x)
        Z_PARAM_LONG(y)
        Z_PARAM_LONG(z)
        Z_PARAM_OPTIONAL
        Z_PARAM_LONG(x_off)
        Z_PARAM_LONG(y_off)
        Z_PARAM_LONG(z_off)
    ZEND_PARSE_PARAMETERS_END();

    std::vector<double> noises;
    for (int xx = x_off; xx < (x + x_off); xx++) {
        for (int yx = y_off; yx < (y + y_off); yx++) {
            for (int zx = z_off; zx < (z + z_off); zx++) {
                noises.push_back((double) container->GetNoise((float) xx, (float)yx, (float) zx));
            }
        }
    }

    zval *val;
    zend_array *array = zend_new_array(noises.size());
    for (int i = 0; i < noises.size(); ++i) {
        ZVAL_DOUBLE(val, noises[i]);

        zend_hash_index_add(array, i, val);
    }

    RETVAL_ARR(array);
}

// }}

// Register methods to class
zend_function_entry noise_lib_methods[] = {
    PHP_ME(noiselib, __construct, arginfo___construct, ZEND_ACC_PUBLIC)
    PHP_ME(noiselib, generate2d, arginfo_gen2d, ZEND_ACC_PUBLIC)
    PHP_ME(noiselib, generate3d, arginfo_gen3d, ZEND_ACC_PUBLIC)
    PHP_FE_END
};

void register_noise_class() {
    zend_class_entry cle;
    INIT_CLASS_ENTRY(cle, "Noise", noise_lib_methods);
    zend_register_internal_class(&cle);
}