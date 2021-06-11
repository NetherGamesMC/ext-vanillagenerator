#ifndef SIMPLEX_OCTAVE_GENERATOR_IMPL
#define SIMPLEX_OCTAVE_GENERATOR_IMPL

#include "SimplexOctaveGeneratorImpl.h"

#endif

zend_class_entry *simplex_octave_entry;
static zend_object_handlers simplex_octave_handlers;

static zend_object* simplex_noise_new(zend_class_entry* class_type) {
    auto object = alloc_custom_zend_object<simplex_octave_obj>(class_type, &simplex_octave_handlers);

    return &object->std;
}

static void simplex_octave_free(zend_object* obj) {
    zend_object_std_dtor(obj);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////// fromRandomOctaves() /////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_SimplexOctaveGenerator_fromRandomAndOctaves, 0, 5, SimplexOctaveGenerator, 0)
    ZEND_ARG_OBJ_INFO(0, seed, Random, 0)
    ZEND_ARG_TYPE_INFO(0, octavesNum, IS_LONG, 0)
    ZEND_ARG_TYPE_INFO(0, size_x, IS_LONG, 0)
    ZEND_ARG_TYPE_INFO(0, size_y, IS_LONG, 0)
    ZEND_ARG_TYPE_INFO(0, size_z, IS_LONG, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(SimplexOctaveGenerator, fromRandomAndOctaves) {
    zend_long octavesNum, size_x, size_y, size_z;

    zval *seed;
    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 5, 5)
        Z_PARAM_OBJECT_OF_CLASS(seed, random_entry)
        Z_PARAM_LONG(octavesNum)
        Z_PARAM_LONG(size_x)
        Z_PARAM_LONG(size_y)
        Z_PARAM_LONG(size_z)
    ZEND_PARSE_PARAMETERS_END();

    object_init_ex(return_value, simplex_octave_entry);

    auto object = fetch_from_zend_object<simplex_octave_obj>(Z_OBJ_P(return_value));
    auto random = fetch_from_zend_object<random_obj>(Z_OBJ_P(seed));

    new (&object->simplexOctave) SimplexOctaveGenerator(random->random, static_cast<int>(octavesNum), static_cast<int>(size_x), static_cast<int>(size_y), static_cast<int>(size_z));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////// getFractalBrownianMotion() /////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_SimplexOctaveGenerator_getFractalBrownianMotion, 0, 6, IS_VOID, 0)
    ZEND_ARG_TYPE_INFO(1, array, IS_ARRAY, 0)
    ZEND_ARG_TYPE_INFO(0, x, IS_DOUBLE, 0)
    ZEND_ARG_TYPE_INFO(0, y, IS_DOUBLE, 0)
    ZEND_ARG_TYPE_INFO(0, z, IS_DOUBLE, 0)
    ZEND_ARG_TYPE_INFO(0, lacunarity, IS_DOUBLE, 0)
    ZEND_ARG_TYPE_INFO(0, persistence, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(SimplexOctaveGenerator, getFractalBrownianMotion){
    zval *array;
    double x, y, z, lacunarity, persistence;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 6, 6)
        Z_PARAM_ARRAY_EX(array, 1, 1)
        Z_PARAM_DOUBLE(x)
        Z_PARAM_DOUBLE(y)
        Z_PARAM_DOUBLE(z)
        Z_PARAM_DOUBLE(lacunarity)
        Z_PARAM_DOUBLE(persistence)
    ZEND_PARSE_PARAMETERS_END();

    auto object = fetch_from_zend_object<simplex_octave_obj>(Z_OBJ_P(getThis()));

    SimplexOctaveGenerator octave = object->simplexOctave;

    double *pointer = octave.getFractalBrownianMotion((float)x, (float)y, (float)z, (float)lacunarity, (float)persistence);

    zval noise;
    zend_array *zendArray = Z_ARRVAL_P(array);
    for (int i = 0; i < octave.getArraySize(); ++i) {
        ZVAL_DOUBLE(&noise, pointer[i]);

        if (zend_hash_index_exists(zendArray, i)) {
            zend_hash_index_update(zendArray, i, &noise);
        } else {
            zend_hash_index_add(zendArray, i, &noise);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////// noise() /////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_SimplexOctaveGenerator_noise, 0, 6, IS_DOUBLE, 0)
    ZEND_ARG_TYPE_INFO(0, x, IS_DOUBLE, 0)
    ZEND_ARG_TYPE_INFO(0, y, IS_DOUBLE, 0)
    ZEND_ARG_TYPE_INFO(0, z, IS_DOUBLE, 0)
    ZEND_ARG_TYPE_INFO(0, frequency, IS_DOUBLE, 0)
    ZEND_ARG_TYPE_INFO(0, amplitude, IS_DOUBLE, 0)
    ZEND_ARG_TYPE_INFO(0, normalized, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(SimplexOctaveGenerator, noise){
    bool normalized;
    double x, y, z, frequency, amplitude;
    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 6, 6)
        Z_PARAM_DOUBLE(x)
        Z_PARAM_DOUBLE(y)
        Z_PARAM_DOUBLE(z)
        Z_PARAM_DOUBLE(frequency)
        Z_PARAM_DOUBLE(amplitude)
        Z_PARAM_BOOL(normalized)
    ZEND_PARSE_PARAMETERS_END();

    auto object = fetch_from_zend_object<simplex_octave_obj>(Z_OBJ_P(getThis()));

    RETURN_DOUBLE(object->simplexOctave.noise(x, y, z, frequency, amplitude, normalized));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////// setScale() ///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_SimplexOctaveGenerator_setScale, 0, 1, IS_VOID, 0)
    ZEND_ARG_TYPE_INFO(0, scale, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(SimplexOctaveGenerator, setScale){
    double scale;
    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
        Z_PARAM_DOUBLE(scale)
    ZEND_PARSE_PARAMETERS_END();

    auto object = fetch_from_zend_object<simplex_octave_obj>(Z_OBJ_P(getThis()));

    object->simplexOctave.setScale((float)scale);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////// getSizeX() ////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_SimplexOctaveGenerator_getSizeX, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(SimplexOctaveGenerator, getSizeX){
    zend_parse_parameters_none_throw();

    auto object = fetch_from_zend_object<simplex_octave_obj>(Z_OBJ_P(getThis()));

    RETURN_LONG(object->simplexOctave.getSizeX());
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////// getSizeZ() ////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_SimplexOctaveGenerator_getSizeZ, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(SimplexOctaveGenerator, getSizeZ){
    zend_parse_parameters_none_throw();

    auto object = fetch_from_zend_object<simplex_octave_obj>(Z_OBJ_P(getThis()));

    RETURN_LONG(object->simplexOctave.getSizeZ());
}

// Register methods to class
zend_function_entry simplex_octave_methods[] = {
    PHP_ME(SimplexOctaveGenerator, fromRandomAndOctaves, arginfo_SimplexOctaveGenerator_fromRandomAndOctaves, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_ME(SimplexOctaveGenerator, setScale, arginfo_SimplexOctaveGenerator_setScale, ZEND_ACC_PUBLIC)
    PHP_ME(SimplexOctaveGenerator, getSizeX, arginfo_SimplexOctaveGenerator_getSizeX, ZEND_ACC_PUBLIC)
    PHP_ME(SimplexOctaveGenerator, getSizeZ, arginfo_SimplexOctaveGenerator_getSizeZ, ZEND_ACC_PUBLIC)
    PHP_ME(SimplexOctaveGenerator, getFractalBrownianMotion, arginfo_SimplexOctaveGenerator_getFractalBrownianMotion, ZEND_ACC_PUBLIC)
    PHP_ME(SimplexOctaveGenerator, noise, arginfo_SimplexOctaveGenerator_noise, ZEND_ACC_PUBLIC)
    PHP_FE_END
};

void register_simplex_generator() {
    memcpy(&simplex_octave_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
    simplex_octave_handlers.offset = XtOffsetOf(simplex_octave_obj, std);
    simplex_octave_handlers.free_obj = simplex_octave_free;

    zend_class_entry cle;
    INIT_CLASS_ENTRY(cle, "SimplexOctaveGenerator", simplex_octave_methods);
    cle.create_object = simplex_noise_new;
    simplex_octave_entry = zend_register_internal_class(&cle);
    simplex_octave_entry->ce_flags |= ZEND_ACC_FINAL;
}