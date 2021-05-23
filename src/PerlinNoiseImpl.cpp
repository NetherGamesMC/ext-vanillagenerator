#ifndef SIMPLEX_NOISE_INCLUDED
#define SIMPLEX_NOISE_INCLUDED

#include "PerlinNoiseImpl.h"

#endif

typedef struct {
    PerlinNoise perlinNoise;
    zend_object std;
} perlin_noise_obj;

static zend_object_handlers simplex_noise_handlers;

static zend_object* perlin_noise_new(zend_class_entry* class_type) {
    perlin_noise_obj* object = alloc_custom_zend_object<perlin_noise_obj>(class_type, &simplex_noise_handlers);

    return &object->std;
}

static void perlin_noise_free(zend_object* obj) {
    zend_object_std_dtor(obj);
}

// --- FUNCTION: __construct(float $off_x, float $off_y, float $off_z): void

ZEND_BEGIN_ARG_INFO_EX(arginfo___construct, 0, 0, 1)
    ZEND_ARG_TYPE_INFO(0, seed, IS_LONG, 1)
ZEND_END_ARG_INFO()

PHP_METHOD(perlin, __construct) {
    zend_long seed;
    bool isNull = true;

    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_LONG_OR_NULL(seed, isNull)
    ZEND_PARSE_PARAMETERS_END();

    auto object = fetch_from_zend_object<perlin_noise_obj>(Z_OBJ_P(getThis()));

    new (&object->perlinNoise) PerlinNoise();

    object->perlinNoise.init(new Random(seed));
}

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_getNoise, 0, 10, IS_ARRAY, 0)
    ZEND_ARG_TYPE_INFO(0, x, IS_DOUBLE, 0)
    ZEND_ARG_TYPE_INFO(0, y, IS_DOUBLE, 0)
    ZEND_ARG_TYPE_INFO(0, z, IS_DOUBLE, 0)
    ZEND_ARG_TYPE_INFO(0, size_x, IS_LONG, 0)
    ZEND_ARG_TYPE_INFO(0, size_y, IS_LONG, 0)
    ZEND_ARG_TYPE_INFO(0, size_z, IS_LONG, 0)
    ZEND_ARG_TYPE_INFO(0, scale_x, IS_DOUBLE, 0)
    ZEND_ARG_TYPE_INFO(0, scale_y, IS_DOUBLE, 0)
    ZEND_ARG_TYPE_INFO(0, scale_z, IS_DOUBLE, 0)
    ZEND_ARG_TYPE_INFO(0, amplitude, IS_DOUBLE, 0)
ZEND_END_ARG_INFO();

PHP_METHOD(perlin, getNoise) {
    auto container = fetch_from_zend_object<perlin_noise_obj>(Z_OBJ_P(getThis()));

    zend_long size_x, size_y, size_z;
    double xin, y, z, scale_x, scale_y, scale_z, amplitude;
    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 10, 10)
        Z_PARAM_DOUBLE(xin)
        Z_PARAM_DOUBLE(y)
        Z_PARAM_DOUBLE(z)
        Z_PARAM_LONG(size_x)
        Z_PARAM_LONG(size_y)
        Z_PARAM_LONG(size_z)
        Z_PARAM_DOUBLE(scale_x)
        Z_PARAM_DOUBLE(scale_y)
        Z_PARAM_DOUBLE(scale_z)
        Z_PARAM_DOUBLE(amplitude)
    ZEND_PARSE_PARAMETERS_END();

    std::vector<double> noises;
    for (const float &noiseData : container->perlinNoise.getNoise((float)xin, (float)y, (float)z, size_x, size_y, size_z, (float)scale_x, (float)scale_y, (float)scale_z, (float)amplitude)){
        noises.push_back((double) noiseData);
    }

    zval *val;
    zend_array *array = zend_new_array(noises.size());
    for (int i = 0; i < noises.size(); ++i) {
        ZVAL_DOUBLE(val, noises[i]);

        zend_hash_index_add(array, i, val);
    }

    RETURN_ARR(array);
}

// Register methods to class
zend_function_entry perlin_noise_methods[] = {
    PHP_ME(perlin, __construct, arginfo___construct, ZEND_ACC_PUBLIC)
    PHP_ME(perlin, getNoise, arginfo_getNoise, ZEND_ACC_PUBLIC)
    PHP_FE_END
};

void register_noise_class() {
    memcpy(&simplex_noise_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
    simplex_noise_handlers.offset = XtOffsetOf(perlin_noise_obj, std);
    simplex_noise_handlers.free_obj = perlin_noise_free;

    zend_class_entry cle;
    INIT_CLASS_ENTRY(cle, "PerlinNoise", perlin_noise_methods);
    cle.create_object = perlin_noise_new;
    zend_register_internal_class(&cle);
}