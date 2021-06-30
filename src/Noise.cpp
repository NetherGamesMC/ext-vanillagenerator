#include "Noise.h"

zend_class_entry *perlin_octave_entry;
zend_class_entry *noise_map_entry;
zend_class_entry *simplex_octave_entry;

static zend_object_handlers noise_map_handlers;

static zend_object *map_noise_new(zend_class_entry *class_type) {
    auto object = alloc_custom_zend_object<noise_map_obj>(class_type, &noise_map_handlers);

    return &object->std;
}

static void noise_map_free(zend_object *obj) {
    zend_object_std_dtor(obj);
}

// --
// {{{ # BEGIN CLASS      NoiseMapGenerator
// --

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////// fromRandomOctaves() /////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_NoiseMapGenerator_fromRandomAndOctaves, 0, 2, NoiseMapGenerator, 0)
    ZEND_ARG_OBJ_INFO(0, seed, Random, 0)
    ZEND_ARG_TYPE_INFO(0, octavesNum, IS_LONG, 0)
ZEND_END_ARG_INFO()

PHP_METHOD (NoiseMapGenerator, fromRandomAndOctaves) {
    zend_long octavesNum;

    zval *seed;
    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 2, 2)
        Z_PARAM_OBJECT_OF_CLASS(seed, random_entry)
        Z_PARAM_LONG(octavesNum)
    ZEND_PARSE_PARAMETERS_END();

    object_init_ex(return_value, noise_map_entry);

    auto object = fetch_from_zend_object<noise_map_obj>(Z_OBJ_P(return_value));
    auto random = fetch_from_zend_object<random_obj>(Z_OBJ_P(seed));

    new(&object->bukkitSimplexOctave) BukkitSimplexOctaveGenerator(random->random, static_cast<int>(octavesNum));
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

PHP_METHOD (NoiseMapGenerator, noise) {
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

    RETURN_DOUBLE(object->bukkitSimplexOctave.noise(x, y, frequency, amplitude, normalized));
}

// --
// {{{ # BEGIN CLASS      PerlinOctaveGenerator
// --

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////// fromRandomOctaves() /////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_PerlinOctaveGenerator_fromRandomAndOctaves, 0, 5, PerlinOctaveGenerator,
                                       0)
    ZEND_ARG_OBJ_INFO(0, seed, Random, 0)
    ZEND_ARG_TYPE_INFO(0, octavesNum, IS_LONG, 0)
    ZEND_ARG_TYPE_INFO(0, size_x, IS_LONG, 0)
    ZEND_ARG_TYPE_INFO(0, size_y, IS_LONG, 0)
    ZEND_ARG_TYPE_INFO(0, size_z, IS_LONG, 0)
ZEND_END_ARG_INFO()

PHP_METHOD (PerlinOctaveGenerator, fromRandomAndOctaves) {
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

    auto object = fetch_from_zend_object<noise_map_obj>(Z_OBJ_P(return_value));
    auto random = fetch_from_zend_object<random_obj>(Z_OBJ_P(seed));

    new(&object->perlinOctave) PerlinOctaveGenerator(random->random, static_cast<int>(octavesNum), static_cast<int>(size_x), static_cast<int>(size_y), static_cast<int>(size_z));
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

PHP_METHOD (PerlinOctaveGenerator, getFractalBrownianMotion) {
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

    auto object = fetch_from_zend_object<noise_map_obj>(Z_OBJ_P(getThis()));

    PerlinOctaveGenerator octave = object->perlinOctave;

    std::vector<double> pointer = octave.getFractalBrownianMotion(x, y, z, lacunarity, persistence);

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

PHP_METHOD (PerlinOctaveGenerator, setXScale) {
    double x_scale;
    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
        Z_PARAM_DOUBLE(x_scale)
    ZEND_PARSE_PARAMETERS_END();

    auto object = fetch_from_zend_object<noise_map_obj>(Z_OBJ_P(getThis()));

    object->perlinOctave.setXScale((float) x_scale);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////// setYScale() ///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_PerlinOctaveGenerator_setYScale, 0, 1, IS_VOID, 0)
    ZEND_ARG_TYPE_INFO(0, x_scale, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

PHP_METHOD (PerlinOctaveGenerator, setYScale) {
    double y_scale;
    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
        Z_PARAM_DOUBLE(y_scale)
    ZEND_PARSE_PARAMETERS_END();

    auto object = fetch_from_zend_object<noise_map_obj>(Z_OBJ_P(getThis()));

    object->perlinOctave.setYScale((float) y_scale);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////// setZScale() ///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_PerlinOctaveGenerator_setZScale, 0, 1, IS_VOID, 0)
    ZEND_ARG_TYPE_INFO(0, x_scale, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

PHP_METHOD (PerlinOctaveGenerator, setZScale) {
    double z_scale;
    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
        Z_PARAM_DOUBLE(z_scale)
    ZEND_PARSE_PARAMETERS_END();

    auto object = fetch_from_zend_object<noise_map_obj>(Z_OBJ_P(getThis()));

    object->perlinOctave.setZScale((float) z_scale);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////// getSizeY() ////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_PerlinOctaveGenerator_getSizeY, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

PHP_METHOD (PerlinOctaveGenerator, getSizeY) {
    zend_parse_parameters_none_throw();

    auto object = fetch_from_zend_object<noise_map_obj>(Z_OBJ_P(getThis()));

    RETURN_LONG(object->perlinOctave.getSizeY());
}

// --
// {{{ # BEGIN CLASS      SimplexOctaveGenerator
// --

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

PHP_METHOD (SimplexOctaveGenerator, fromRandomAndOctaves) {
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

    auto object = fetch_from_zend_object<noise_map_obj>(Z_OBJ_P(return_value));
    auto random = fetch_from_zend_object<random_obj>(Z_OBJ_P(seed));

    new(&object->simplexOctave) SimplexOctaveGenerator(random->random, static_cast<int>(octavesNum), static_cast<int>(size_x), static_cast<int>(size_y), static_cast<int>(size_z));
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

PHP_METHOD (SimplexOctaveGenerator, getFractalBrownianMotion) {
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

    auto object = fetch_from_zend_object<noise_map_obj>(Z_OBJ_P(getThis()));

    SimplexOctaveGenerator octave = object->simplexOctave;

    std::vector<double> pointer = octave.getFractalBrownianMotion((float) x, (float) y, (float) z, (float) lacunarity, (float) persistence);

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

PHP_METHOD (SimplexOctaveGenerator, noise) {
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

    auto object = fetch_from_zend_object<noise_map_obj>(Z_OBJ_P(getThis()));

    RETURN_DOUBLE(object->simplexOctave.noise(x, y, z, frequency, amplitude, normalized));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////// setScale() ///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_SimplexOctaveGenerator_setScale, 0, 1, IS_VOID, 0)
    ZEND_ARG_TYPE_INFO(0, scale, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

PHP_METHOD (SimplexOctaveGenerator, setScale) {
    double scale;
    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
        Z_PARAM_DOUBLE(scale)
    ZEND_PARSE_PARAMETERS_END();

    auto object = fetch_from_zend_object<noise_map_obj>(Z_OBJ_P(getThis()));

    object->simplexOctave.setScale((float) scale);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////// getSizeX() ////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_SimplexOctaveGenerator_getSizeX, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

PHP_METHOD (SimplexOctaveGenerator, getSizeX) {
    zend_parse_parameters_none_throw();

    auto object = fetch_from_zend_object<noise_map_obj>(Z_OBJ_P(getThis()));

    RETURN_LONG(object->simplexOctave.getSizeX());
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////// getSizeZ() ////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_SimplexOctaveGenerator_getSizeZ, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

PHP_METHOD (SimplexOctaveGenerator, getSizeZ) {
    zend_parse_parameters_none_throw();

    auto object = fetch_from_zend_object<noise_map_obj>(Z_OBJ_P(getThis()));

    RETURN_LONG(object->simplexOctave.getSizeZ());
}

// --
// {{{ # START OF ZEND_FUNCTION_ENTRIES
// --

zend_function_entry simplex_octave_methods[] = {
    PHP_ME(SimplexOctaveGenerator, fromRandomAndOctaves, arginfo_SimplexOctaveGenerator_fromRandomAndOctaves, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_ME(SimplexOctaveGenerator, setScale, arginfo_SimplexOctaveGenerator_setScale, ZEND_ACC_PUBLIC)
    PHP_ME(SimplexOctaveGenerator, getSizeX, arginfo_SimplexOctaveGenerator_getSizeX, ZEND_ACC_PUBLIC)
    PHP_ME(SimplexOctaveGenerator, getSizeZ, arginfo_SimplexOctaveGenerator_getSizeZ, ZEND_ACC_PUBLIC)
    PHP_ME(SimplexOctaveGenerator, getFractalBrownianMotion, arginfo_SimplexOctaveGenerator_getFractalBrownianMotion, ZEND_ACC_PUBLIC)
    PHP_ME(SimplexOctaveGenerator, noise, arginfo_SimplexOctaveGenerator_noise, ZEND_ACC_PUBLIC)
    PHP_FE_END
};

zend_function_entry perlin_octave_methods[] = {
    PHP_ME(PerlinOctaveGenerator, fromRandomAndOctaves, arginfo_PerlinOctaveGenerator_fromRandomAndOctaves, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_ME(PerlinOctaveGenerator, setXScale, arginfo_PerlinOctaveGenerator_setXScale, ZEND_ACC_PUBLIC)
    PHP_ME(PerlinOctaveGenerator, setYScale, arginfo_PerlinOctaveGenerator_setYScale, ZEND_ACC_PUBLIC)
    PHP_ME(PerlinOctaveGenerator, setZScale, arginfo_PerlinOctaveGenerator_setZScale, ZEND_ACC_PUBLIC)
    PHP_ME(PerlinOctaveGenerator, getFractalBrownianMotion, arginfo_PerlinOctaveGenerator_getFractalBrownianMotion, ZEND_ACC_PUBLIC)
    PHP_ME(PerlinOctaveGenerator, getSizeY, arginfo_PerlinOctaveGenerator_getSizeY, ZEND_ACC_PUBLIC)
    PHP_FE_END
};

zend_function_entry noise_map_methods[] = {
    PHP_ME(NoiseMapGenerator, fromRandomAndOctaves, arginfo_NoiseMapGenerator_fromRandomAndOctaves, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_ME(NoiseMapGenerator, noise, arginfo_NoiseMapGenerator_noise, ZEND_ACC_PUBLIC)
    PHP_FE_END
};

// }}} # EOL

void register_noise_function() {
    memcpy(&noise_map_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
    noise_map_handlers.offset = XtOffsetOf(noise_map_obj, std);
    noise_map_handlers.free_obj = noise_map_free;

    zend_class_entry perlinEntry;
    INIT_CLASS_ENTRY(perlinEntry, "PerlinOctaveGenerator", perlin_octave_methods);
    perlinEntry.create_object = map_noise_new;
    perlin_octave_entry = zend_register_internal_class(&perlinEntry);
    perlin_octave_entry->ce_flags |= ZEND_ACC_FINAL;

    zend_class_entry bukkitEntry;
    INIT_CLASS_ENTRY(bukkitEntry, "NoiseMapGenerator", noise_map_methods);
    bukkitEntry.create_object = map_noise_new;
    noise_map_entry = zend_register_internal_class(&bukkitEntry);
    noise_map_entry->ce_flags |= ZEND_ACC_FINAL;

    zend_class_entry simplexEntry;
    INIT_CLASS_ENTRY(simplexEntry, "SimplexOctaveGenerator", simplex_octave_methods);
    simplexEntry.create_object = map_noise_new;
    simplex_octave_entry = zend_register_internal_class(&simplexEntry);
    simplex_octave_entry->ce_flags |= ZEND_ACC_FINAL;
}