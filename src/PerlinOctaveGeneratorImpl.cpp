#include "PerlinOctaveGeneratorImpl.h"

zend_class_entry *perlin_octave_entry;
static zend_object_handlers perlin_octave_handlers;

static zend_object* perlin_noise_new(zend_class_entry* class_type) {
    auto object = alloc_custom_zend_object<perlin_octave_obj>(class_type, &perlin_octave_handlers);

    return &object->std;
}

static void perlin_octave_free(zend_object* obj) {
    zend_object_std_dtor(obj);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////// fromRandomOctaves() /////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_PerlinOctaveGenerator_fromRandomAndOctaves, 0, 5, PerlinOctaveGenerator, 0)
    ZEND_ARG_OBJ_INFO(0, seed, Random, 0)
    ZEND_ARG_TYPE_INFO(0, octavesNum, IS_LONG, 0)
    ZEND_ARG_TYPE_INFO(0, size_x, IS_LONG, 0)
    ZEND_ARG_TYPE_INFO(0, size_y, IS_LONG, 0)
    ZEND_ARG_TYPE_INFO(0, size_z, IS_LONG, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(PerlinOctaveGenerator, fromRandomAndOctaves) {
    zend_long octavesNum, size_x, size_y, size_z;

    zval *seed;
    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 5, 5)
        Z_PARAM_OBJECT_OF_CLASS(seed, random_entry)
        Z_PARAM_LONG(octavesNum)
        Z_PARAM_LONG(size_x)
        Z_PARAM_LONG(size_y)
        Z_PARAM_LONG(size_z)
    ZEND_PARSE_PARAMETERS_END();

    object_init_ex(return_value, perlin_octave_entry);

    auto object = fetch_from_zend_object<perlin_octave_obj>(Z_OBJ_P(return_value));
    auto random = fetch_from_zend_object<random_obj>(Z_OBJ_P(seed));

    new (&object->perlinOctave) PerlinOctaveGenerator(random->random, static_cast<int>(octavesNum), static_cast<int>(size_x), static_cast<int>(size_y), static_cast<int>(size_z));
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////// getFractalBrownianMotion() /////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_PerlinOctaveGenerator_getFractalBrownianMotion, 0, 6, IS_VOID, 0)
    ZEND_ARG_TYPE_INFO(1, array, IS_ARRAY, 0)
    ZEND_ARG_TYPE_INFO(0, x, IS_DOUBLE, 0)
    ZEND_ARG_TYPE_INFO(0, y, IS_DOUBLE, 0)
    ZEND_ARG_TYPE_INFO(0, z, IS_DOUBLE, 0)
    ZEND_ARG_TYPE_INFO(0, lacunarity, IS_DOUBLE, 0)
    ZEND_ARG_TYPE_INFO(0, persistence, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(PerlinOctaveGenerator, getFractalBrownianMotion){
    zval *array;
    double x, y, z, lacunarity, persistence;
    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 6, 6)
        Z_PARAM_ARRAY_EX(array, 0, 1)
        Z_PARAM_DOUBLE(x)
        Z_PARAM_DOUBLE(y)
        Z_PARAM_DOUBLE(z)
        Z_PARAM_DOUBLE(lacunarity)
        Z_PARAM_DOUBLE(persistence)
    ZEND_PARSE_PARAMETERS_END();

    auto object = fetch_from_zend_object<perlin_octave_obj>(Z_OBJ_P(getThis()));

    PerlinOctaveGenerator octave = object->perlinOctave;

    double *pointer = octave.getFractalBrownianMotion(x, y, z, lacunarity, persistence);

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
/////////////////////////////////////////// setXScale() ///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_PerlinOctaveGenerator_setXScale, 0, 1, IS_VOID, 0)
    ZEND_ARG_TYPE_INFO(0, x_scale, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(PerlinOctaveGenerator, setXScale){
    double x_scale;
    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
        Z_PARAM_DOUBLE(x_scale)
    ZEND_PARSE_PARAMETERS_END();

    auto object = fetch_from_zend_object<perlin_octave_obj>(Z_OBJ_P(getThis()));

    object->perlinOctave.setXScale((float)x_scale);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////// setYScale() ///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_PerlinOctaveGenerator_setYScale, 0, 1, IS_VOID, 0)
    ZEND_ARG_TYPE_INFO(0, x_scale, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(PerlinOctaveGenerator, setYScale){
    double y_scale;
    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
        Z_PARAM_DOUBLE(y_scale)
    ZEND_PARSE_PARAMETERS_END();

    auto object = fetch_from_zend_object<perlin_octave_obj>(Z_OBJ_P(getThis()));

    object->perlinOctave.setYScale((float)y_scale);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////// setZScale() ///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_PerlinOctaveGenerator_setZScale, 0, 1, IS_VOID, 0)
    ZEND_ARG_TYPE_INFO(0, x_scale, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(PerlinOctaveGenerator, setZScale){
    double z_scale;
    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
        Z_PARAM_DOUBLE(z_scale)
    ZEND_PARSE_PARAMETERS_END();

    auto object = fetch_from_zend_object<perlin_octave_obj>(Z_OBJ_P(getThis()));

    object->perlinOctave.setZScale((float)z_scale);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////// getSizeY() ////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_PerlinOctaveGenerator_getSizeY, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(PerlinOctaveGenerator, getSizeY){
    zend_parse_parameters_none_throw();

    auto object = fetch_from_zend_object<perlin_octave_obj>(Z_OBJ_P(getThis()));

    RETURN_LONG(object->perlinOctave.getSizeY());
}

// Register methods to class
zend_function_entry perlin_octave_methods[] = {
    PHP_ME(PerlinOctaveGenerator, fromRandomAndOctaves, arginfo_PerlinOctaveGenerator_fromRandomAndOctaves, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_ME(PerlinOctaveGenerator, setXScale, arginfo_PerlinOctaveGenerator_setXScale, ZEND_ACC_PUBLIC)
    PHP_ME(PerlinOctaveGenerator, setYScale, arginfo_PerlinOctaveGenerator_setYScale, ZEND_ACC_PUBLIC)
    PHP_ME(PerlinOctaveGenerator, setZScale, arginfo_PerlinOctaveGenerator_setZScale, ZEND_ACC_PUBLIC)
    PHP_ME(PerlinOctaveGenerator, getFractalBrownianMotion, arginfo_PerlinOctaveGenerator_getFractalBrownianMotion, ZEND_ACC_PUBLIC)
    PHP_ME(PerlinOctaveGenerator, getSizeY, arginfo_PerlinOctaveGenerator_getSizeY, ZEND_ACC_PUBLIC)
    PHP_FE_END
};

void register_perlin_generator() {
    memcpy(&perlin_octave_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
    perlin_octave_handlers.offset = XtOffsetOf(perlin_octave_obj, std);
    perlin_octave_handlers.free_obj = perlin_octave_free;

    zend_class_entry cle;
    INIT_CLASS_ENTRY(cle, "PerlinOctaveGenerator", perlin_octave_methods);
    cle.create_object = perlin_noise_new;
    perlin_octave_entry = zend_register_internal_class(&cle);
    perlin_octave_entry->ce_flags |= ZEND_ACC_FINAL;
}