#ifndef SIMPLEX_OCTAVE_GENERATOR_IMPL
#define SIMPLEX_OCTAVE_GENERATOR_IMPL

#include "SimplexOctaveGeneratorImpl.h"

#endif

zend_class_entry *simplex_octave_entry;

typedef struct {
    SimplexOctaveGenerator simplexOctave;
    zend_object std;
} simplex_octave_obj;

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

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_fromRandomAndOctaves, 0, 5, "SimplexOctaveGenerator", 0)
    ZEND_ARG_TYPE_INFO(0, seed, IS_LONG, 0)
    ZEND_ARG_TYPE_INFO(0, octavesNum, IS_LONG, 0)
    ZEND_ARG_TYPE_INFO(0, size_x, IS_LONG, 0)
    ZEND_ARG_TYPE_INFO(0, size_y, IS_LONG, 0)
    ZEND_ARG_TYPE_INFO(0, size_z, IS_LONG, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(simplex_octave, fromRandomAndOctaves) {
    zend_long seed, octavesNum, size_x, size_y, size_z;

    // zend_l, int
    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 5, 5)
        Z_PARAM_LONG(seed)
        Z_PARAM_LONG(octavesNum)
        Z_PARAM_LONG(size_x)
        Z_PARAM_LONG(size_y)
        Z_PARAM_LONG(size_z)
    ZEND_PARSE_PARAMETERS_END();

    object_init_ex(return_value, simplex_octave_entry);

    auto object = fetch_from_zend_object<simplex_octave_obj>(Z_OBJ_P(return_value));

    new (&object->simplexOctave) SimplexOctaveGenerator();

    object->simplexOctave.init(new Random(seed), octavesNum, size_x, size_y, size_z);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////// getFractalBrownianMotion() /////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_getFractalBrownianMotion, 0, 5, IS_ARRAY, 0)
    ZEND_ARG_TYPE_INFO(0, x, IS_LONG, 0)
    ZEND_ARG_TYPE_INFO(0, y, IS_LONG, 0)
    ZEND_ARG_TYPE_INFO(0, z, IS_LONG, 0)
    ZEND_ARG_TYPE_INFO(0, lacunarity, IS_LONG, 0)
    ZEND_ARG_TYPE_INFO(0, persistence, IS_LONG, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(simplex_octave, getFractalBrownianMotion){
    double x, y, z, lacunarity, persistence;
    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 5, 5)
        Z_PARAM_DOUBLE(x)
        Z_PARAM_DOUBLE(y)
        Z_PARAM_DOUBLE(z)
        Z_PARAM_DOUBLE(lacunarity)
        Z_PARAM_DOUBLE(persistence)
    ZEND_PARSE_PARAMETERS_END();

    auto object = fetch_from_zend_object<simplex_octave_obj>(Z_OBJ_P(getThis()));

    std::vector<double> noises;
    for (const float &noiseData : object->simplexOctave.getFractalBrownianMotion((float)x, (float)y, (float)z, (float)lacunarity, (float)persistence)){
        noises.push_back((double) noiseData);
    }

    zval *val;
    zend_array *array = zend_new_array(noises.size());
    for (int i = 0; i < noises.size(); ++i) {
        ZVAL_DOUBLE(val, noises[i]);

        zend_hash_index_add(array, i, val);
    }

    php_printf("DONE\r\n");

    RETURN_ARR(array);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////// setXScale() ///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_setXScale, 0, 1, IS_VOID, 0)
    ZEND_ARG_TYPE_INFO(0, x_scale, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(simplex_octave, setXScale){
    double x_scale;
    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
        Z_PARAM_DOUBLE(x_scale)
    ZEND_PARSE_PARAMETERS_END();

    auto object = fetch_from_zend_object<simplex_octave_obj>(Z_OBJ_P(getThis()));

    object->simplexOctave.setXScale((float)x_scale);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////// setYScale() ///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_setYScale, 0, 1, IS_VOID, 0)
    ZEND_ARG_TYPE_INFO(0, x_scale, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(simplex_octave, setYScale){
    double y_scale;
    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
        Z_PARAM_DOUBLE(y_scale)
    ZEND_PARSE_PARAMETERS_END();

    auto object = fetch_from_zend_object<simplex_octave_obj>(Z_OBJ_P(getThis()));

    object->simplexOctave.setYScale((float)y_scale);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////// setZScale() ///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_setZScale, 0, 1, IS_VOID, 0)
    ZEND_ARG_TYPE_INFO(0, x_scale, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(simplex_octave, setZScale){
    double z_scale;
    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
        Z_PARAM_DOUBLE(z_scale)
    ZEND_PARSE_PARAMETERS_END();

    auto object = fetch_from_zend_object<simplex_octave_obj>(Z_OBJ_P(getThis()));

    object->simplexOctave.setZScale((float)z_scale);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////// getSizeX() ////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_getSizeX, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(simplex_octave, getSizeX){
    zend_parse_parameters_none_throw();

    auto object = fetch_from_zend_object<simplex_octave_obj>(Z_OBJ_P(getThis()));

    RETURN_LONG(object->simplexOctave.getSizeX());
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////// getSizeY() ////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_getSizeY, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(simplex_octave, getSizeY){
    zend_parse_parameters_none_throw();

    auto object = fetch_from_zend_object<simplex_octave_obj>(Z_OBJ_P(getThis()));

    RETURN_LONG(object->simplexOctave.getSizeY());
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////// getSizeZ() ////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_getSizeZ, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(simplex_octave, getSizeZ){
    zend_parse_parameters_none_throw();

    auto object = fetch_from_zend_object<simplex_octave_obj>(Z_OBJ_P(getThis()));

    RETURN_LONG(object->simplexOctave.getSizeZ());
}

// Register methods to class
zend_function_entry simplex_octave_methods[] = {
    PHP_ME(simplex_octave, fromRandomAndOctaves, arginfo_fromRandomAndOctaves, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_ME(simplex_octave, setXScale, arginfo_setXScale, ZEND_ACC_PUBLIC)
    PHP_ME(simplex_octave, setYScale, arginfo_setYScale, ZEND_ACC_PUBLIC)
    PHP_ME(simplex_octave, setZScale, arginfo_setZScale, ZEND_ACC_PUBLIC)
    PHP_ME(simplex_octave, getSizeX, arginfo_getSizeX, ZEND_ACC_PUBLIC)
    PHP_ME(simplex_octave, getSizeY, arginfo_getSizeY, ZEND_ACC_PUBLIC)
    PHP_ME(simplex_octave, getSizeZ, arginfo_getSizeZ, ZEND_ACC_PUBLIC)
    PHP_ME(simplex_octave, getFractalBrownianMotion, arginfo_getFractalBrownianMotion, ZEND_ACC_PUBLIC)
    PHP_FE_END
};

void register_simplex_octave() {
    memcpy(&simplex_octave_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
    simplex_octave_handlers.offset = XtOffsetOf(simplex_octave_obj, std);
    simplex_octave_handlers.free_obj = simplex_octave_free;

    zend_class_entry cle;
    INIT_CLASS_ENTRY(cle, "SimplexOctaveGenerator", simplex_octave_methods);
    cle.create_object = simplex_noise_new;
    simplex_octave_entry = zend_register_internal_class(&cle);
    simplex_octave_entry->ce_flags |= ZEND_ACC_FINAL;
}