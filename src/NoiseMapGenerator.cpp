#include "NoiseMapGenerator.h"

zend_class_entry *noise_map_entry;
static zend_object_handlers noise_map_handlers;

static zend_object* map_noise_new(zend_class_entry* class_type) {
    auto object = alloc_custom_zend_object<noise_map_obj>(class_type, &noise_map_handlers);

    return &object->std;
}

static void noise_map_free(zend_object* obj) {
    zend_object_std_dtor(obj);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////// fromRandomOctaves() /////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_NoiseMapGenerator_fromRandomAndOctaves, 0, 2, NoiseMapGenerator, 0)
    ZEND_ARG_OBJ_INFO(0, seed, Random, 0)
    ZEND_ARG_TYPE_INFO(0, octavesNum, IS_LONG, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(NoiseMapGenerator, fromRandomAndOctaves) {
    zend_long octavesNum;

    zval *seed;
    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 2, 2)
        Z_PARAM_OBJECT_OF_CLASS(seed, random_entry)
        Z_PARAM_LONG(octavesNum)
    ZEND_PARSE_PARAMETERS_END();

    object_init_ex(return_value, noise_map_entry);

    auto object = fetch_from_zend_object<noise_map_obj>(Z_OBJ_P(return_value));
    auto random = fetch_from_zend_object<random_obj>(Z_OBJ_P(seed));

    new (&object->simplexOctave) BukkitSimplexOctaveGenerator(random->random, static_cast<int>(octavesNum));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////// noise() /////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_NoiseMapGenerator_noise, 0, 5, IS_DOUBLE, 0)
    ZEND_ARG_TYPE_INFO(0, x, IS_DOUBLE, 0)
    ZEND_ARG_TYPE_INFO(0, y, IS_DOUBLE, 0)
    ZEND_ARG_TYPE_INFO(0, frequency, IS_DOUBLE, 0)
    ZEND_ARG_TYPE_INFO(0, amplitude, IS_DOUBLE, 0)
    ZEND_ARG_TYPE_INFO(0, normalized, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(NoiseMapGenerator, noise){
    bool normalized;
    double x, y, frequency, amplitude;
    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 5, 5)
        Z_PARAM_DOUBLE(x)
        Z_PARAM_DOUBLE(y)
        Z_PARAM_DOUBLE(frequency)
        Z_PARAM_DOUBLE(amplitude)
        Z_PARAM_BOOL(normalized)
    ZEND_PARSE_PARAMETERS_END();

    auto object = fetch_from_zend_object<noise_map_obj>(Z_OBJ_P(getThis()));

    RETURN_DOUBLE(object->simplexOctave.noise(x, y, frequency, amplitude, normalized));
}

zend_function_entry noise_map_methods[] = {
    PHP_ME(NoiseMapGenerator, fromRandomAndOctaves, arginfo_NoiseMapGenerator_fromRandomAndOctaves, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_ME(NoiseMapGenerator, noise, arginfo_NoiseMapGenerator_noise, ZEND_ACC_PUBLIC)
    PHP_FE_END
};

void register_noise_map_generator() {
    memcpy(&noise_map_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
    noise_map_handlers.offset = XtOffsetOf(noise_map_obj, std);
    noise_map_handlers.free_obj = noise_map_free;

    zend_class_entry cle;
    INIT_CLASS_ENTRY(cle, "NoiseMapGenerator", noise_map_methods);
    cle.create_object = map_noise_new;
    noise_map_entry = zend_register_internal_class(&cle);
    noise_map_entry->ce_flags |= ZEND_ACC_FINAL;
}