#include "RandomImpl.h"

zend_class_entry *random_entry;
static zend_object_handlers random_handlers;

static zend_object *random_new(zend_class_entry *class_type) {
    auto object = alloc_custom_zend_object<random_obj>(class_type, &random_handlers);

    return &object->std;
}

static void random_free(zend_object *obj) {
    zend_object_std_dtor(obj);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////// __construct() //////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ZEND_BEGIN_ARG_INFO_EX(arginfo_Random___construct, 0, 0, 0)
    ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, seed, IS_LONG, 0, "-1")
ZEND_END_ARG_INFO()

PHP_METHOD (Random, __construct) {
    zend_long seed = -1;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 0, 1)
        Z_PARAM_OPTIONAL
        Z_PARAM_LONG(seed)
    ZEND_PARSE_PARAMETERS_END();

    if (seed == -1) {
        seed = (zend_long) php_time();
    }

    auto object = fetch_from_zend_object<random_obj>(Z_OBJ_P(getThis()));

    new(&object->random) Random(static_cast<int64_t>(seed));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////// setSeed() ////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_Random_setSeed, 0, 1, IS_VOID, 0)
    ZEND_ARG_TYPE_INFO(0, seed, IS_LONG, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(Random, setSeed) {
    zend_long seed;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
        Z_PARAM_LONG(seed)
    ZEND_PARSE_PARAMETERS_END();

    auto object = fetch_from_zend_object<random_obj>(Z_OBJ_P(getThis()));

    object->random.setSeed(static_cast<int64_t>(seed));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////// getSeed() ////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_Random_getSeed, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(Random, getSeed) {
    zend_parse_parameters_none_throw();

    auto object = fetch_from_zend_object<random_obj>(Z_OBJ_P(getThis()));

    RETURN_LONG(static_cast<zend_long>(object->random.getSeed()));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////// nextInt() ////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_Random_nextInt, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(Random, nextInt) {
    zend_parse_parameters_none_throw();

    auto object = fetch_from_zend_object<random_obj>(Z_OBJ_P(getThis()));

    RETURN_LONG(static_cast<zend_long>(object->random.nextInt()));
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////// nextSignedInt() ////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_Random_nextSignedInt, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(Random, nextSignedInt) {
    zend_parse_parameters_none_throw();

    auto object = fetch_from_zend_object<random_obj>(Z_OBJ_P(getThis()));

    RETURN_LONG(static_cast<zend_long>(object->random.nextSignedInt()));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////// nextFloat() ///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_Random_nextFloat, 0, 0, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(Random, nextFloat) {
    zend_parse_parameters_none_throw();

    auto object = fetch_from_zend_object<random_obj>(Z_OBJ_P(getThis()));

    RETURN_DOUBLE(object->random.nextFloat());
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////// nextSignedFloat() ////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_Random_nextSignedFloat, 0, 0, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(Random, nextSignedFloat) {
    zend_parse_parameters_none_throw();

    auto object = fetch_from_zend_object<random_obj>(Z_OBJ_P(getThis()));

    RETURN_DOUBLE(object->random.nextSignedFloat());
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////// nextBoolean() //////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_Random_nextBoolean, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(Random, nextBoolean) {
    zend_parse_parameters_none_throw();

    auto object = fetch_from_zend_object<random_obj>(Z_OBJ_P(getThis()));

    RETURN_BOOL(object->random.nextBoolean());
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////// nextLong() ///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_Random_nextLong, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(Random, nextLong) {
    zend_parse_parameters_none_throw();

    auto object = fetch_from_zend_object<random_obj>(Z_OBJ_P(getThis()));

    RETURN_LONG(static_cast<zend_long>(object->random.nextLong()));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////// getSeed() ////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_Random_nextRange, 0, 0, IS_LONG, 0)
    ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, start, IS_LONG, 0, "0")
    ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, end, IS_LONG, 0, "0x7fffffff")
ZEND_END_ARG_INFO()

PHP_METHOD(Random, nextRange) {
    zend_long start = 0, end = 0x7fffffff;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 0, 2)
        Z_PARAM_OPTIONAL
        Z_PARAM_LONG(start)
        Z_PARAM_LONG(end)
    ZEND_PARSE_PARAMETERS_END();

    auto object = fetch_from_zend_object<random_obj>(Z_OBJ_P(getThis()));

    RETURN_LONG(static_cast<zend_long>(object->random.nextRange(start, end)));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////// nextBoundedInt() ////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_Random_nextBoundedInt, 0, 1, IS_LONG, 0)
    ZEND_ARG_TYPE_INFO(0, bound, IS_LONG, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(Random, nextBoundedInt) {
    zend_long bound;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
        Z_PARAM_LONG(bound)
    ZEND_PARSE_PARAMETERS_END();

    auto object = fetch_from_zend_object<random_obj>(Z_OBJ_P(getThis()));

    RETURN_LONG(static_cast<zend_long>(object->random.nextBoundedInt(bound)));
}

// Register methods to class
zend_function_entry random_methods[] = {
        PHP_ME(Random, __construct, arginfo_Random___construct, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
        PHP_ME(Random, setSeed, arginfo_Random_setSeed, ZEND_ACC_PUBLIC)
        PHP_ME(Random, getSeed, arginfo_Random_getSeed, ZEND_ACC_PUBLIC)
        PHP_ME(Random, nextInt, arginfo_Random_nextInt, ZEND_ACC_PUBLIC)
        PHP_ME(Random, nextSignedInt, arginfo_Random_nextSignedInt, ZEND_ACC_PUBLIC)
        PHP_ME(Random, nextFloat, arginfo_Random_nextFloat, ZEND_ACC_PUBLIC)
        PHP_ME(Random, nextSignedFloat, arginfo_Random_nextSignedFloat, ZEND_ACC_PUBLIC)
        PHP_ME(Random, nextBoolean, arginfo_Random_nextBoolean, ZEND_ACC_PUBLIC)
        PHP_ME(Random, nextLong, arginfo_Random_nextLong, ZEND_ACC_PUBLIC)
        PHP_ME(Random, nextRange, arginfo_Random_nextRange, ZEND_ACC_PUBLIC)
        PHP_ME(Random, nextBoundedInt, arginfo_Random_nextBoundedInt, ZEND_ACC_PUBLIC)
        PHP_FE_END
};

void register_internal_randoms() {
    memcpy(&random_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
    random_handlers.offset = XtOffsetOf(random_obj, std);
    random_handlers.free_obj = random_free;

    zend_class_entry cle;
    INIT_CLASS_ENTRY(cle, "Random", random_methods);
    cle.create_object = random_new;
    random_entry = zend_register_internal_class(&cle);
}