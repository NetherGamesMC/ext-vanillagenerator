//
// Created by Jack Noordhuis on 1/11/18.
//

#ifndef EXT_POCKETMINE_MATH_ZENDUTIL_H
#define EXT_POCKETMINE_MATH_ZENDUTIL_H

extern "C" {
#include "php.h"
}

#define CLASS_NAME M_CONC(Php, CLASS_TYPE)
#define ARG_INFO_PREFIX M_CONC(arginfo_, M_CONC(CLASS_TYPE, _))

/**
 * A simple struct for keeping track of a zend classes flags.
 */
struct php_class_method_definition {
    uint32_t flags;
};

/**
 * A simple struct for keeping track of the encapsulated C++ and its zend object.
 *
 * @tparam T  The C++ class being encapsulated.
 */
template<class T>
struct class_obj {
    T *container;
    zend_object std;
};

/**
 * A helper function for retrieving a C++ object using a pointer to its zend object.
 *
 * @tparam class_name
 * @param obj
 * @return
 */
template<class T>
static inline class_obj<T> *fetch_from_zend_object(zend_object *obj) {
    return (class_obj<T> *)((char *)obj - XtOffsetOf(class_obj<T>, std));
}

/**
 * A helper function to allocate a zend objects memory.
 *
 * @tparam T          The C++ class being encapsulated.
 * @param class_type  A pointer to the zend class entry.
 * @param handlers    A pointer to the zend object handlers.
 *
 * @return A pointer to the new zend object.
 */
template<class T>
static inline zend_object *allocate_zend_obj(zend_class_entry *class_type, zend_object_handlers *handlers) {
    auto object = (class_obj<T> *)ecalloc(1, sizeof(class_obj<T>) + zend_object_properties_size(class_type));

    zend_object_std_init(&object->std, class_type);
    object_properties_init(&object->std, class_type);

    object->std.handlers = handlers;

    return &object->std;
}

/**
 * A helper function to deallocate a zend objects memory.
 *
 * @tparam T  The C++ class being encapsulated.
 * @param obj A pointer to the zend object being deallocated.
 */
template<class T>
static inline void deallocate_zend_obj(zend_object *obj) {
    auto object = fetch_from_zend_object<T>(obj);
    delete object->container;

    // call the standard free handler
    zend_object_std_dtor(&object->std);
}

/**
 * A helper function to register a class to the zend engine.
 *
 * @tparam T         The C++ class being encapsulated.
 * @param handlers   A pointer to the zend object handlers.
 * @param name       The name/namespace of the class.
 * @param functions  A pointer to the classes method definitions.
 *
 * @return Pointer to the zend class entry.
 */
template<class T>
static inline zend_class_entry *register_zend_class(const char *name, zend_function_entry *functions) {
    zend_class_entry ce;
    INIT_CLASS_ENTRY_EX(ce, name, strlen(name), functions);
    return zend_register_internal_class(&ce);
}

/**
 * A helper function to register a class to the zend engine.
 *
 * @tparam T         The C++ class being encapsulated.
 * @param handlers   A pointer to the zend object handlers.
 * @param name       The name/namespace of the class.
 * @param functions  A pointer to the classes method definitions.
 *
 * @param class_zend_allocation  A pointer to a function that allocates memory for the zend class.
 *
 * @return Pointer to the zend class entry.
 */
template<class T>
static inline zend_class_entry *register_zend_class(zend_object_handlers *handlers, const char *name, zend_function_entry *functions, zend_object *(*class_zend_allocation)(zend_class_entry *)) {
    memcpy(handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
    handlers->offset = XtOffsetOf(class_obj<T>, std);
    handlers->free_obj = deallocate_zend_obj<T>;

    zend_class_entry *ce = register_zend_class<T>(name, functions);
    ce->create_object = class_zend_allocation;
    return ce;
}


 /*******************************************************************
 *                                                                  *
 *     Macros for simplifying the extension development process.    *
 *                                                                  *
 *******************************************************************/


// Macros for concatenating two macro values.
#define M_CONC_(A, B) A##B
#define M_CONC(A, B) M_CONC_(A, B)
#define ZEND_MN_CONC(A, B) M_CONC(zim_, M_CONC_(A, B))


// Macros for the starting definition of a class.

/*
 * Defines class entry pointer.
 */
#define PHP_CLASS_ENTRY() \
    zend_class_entry *M_CONC(CLASS_NAME, _entry)

/*
 * Defines class entry pointer and handler list for constructing php land objects.
 */
#define PHP_CLASS_ENTRY_EX() \
    PHP_CLASS_ENTRY(); \
    zend_object_handlers M_CONC(php_, M_CONC(CLASS_NAME, _handlers)); \
    static zend_object *M_CONC(allocate_, M_CONC(CLASS_NAME, _zend_obj))(zend_class_entry *class_type) { \
        return allocate_zend_obj<CLASS_TYPE>(class_type, &M_CONC(php_, M_CONC(CLASS_NAME, _handlers))); \
    }


// Macros for defining class methods

#define ARG_INFO_NAME(name) \
    M_CONC(ARG_INFO_PREFIX, name)

/*
 * Performs PHP_METHOD functionality without duplicating the class name.
 */
#define PHP_CLASS_METHOD(name) \
    void ZEND_MN_CONC(CLASS_NAME, name)(INTERNAL_FUNCTION_PARAMETERS)

/*
 * Performs PHP_METHOD functionality whilst keeping method flags in the same
 * area of code as method definition.
 */
#define PHP_CLASS_METHOD_EX(name, flags) \
    php_class_method_definition M_CONC(php_, M_CONC(CLASS_NAME, _##name##_method)) = { flags }; \
    void ZEND_MN_CONC(CLASS_NAME, name)(INTERNAL_FUNCTION_PARAMETERS)


// Macros to provide consistent functionality for declaring php land functions that reduce duplicate arguments.

/*
 * Start function entry definition and assign to supplied var name.
 */
#define REGISTER_FUNCTION_ENTRY_START_EX(name) \
    zend_function_entry php_##name[] = {

/*
 * Start function entry definition.
 */
#define REGISTER_FUNCTION_ENTRY_START() \
    zend_function_entry php_methods[] = {

/*
 * Add class function definition.
 */
#define REGISTER_FUNCTION_ENTRY(name) \
    { #name, ZEND_MN_CONC(CLASS_NAME, name), M_CONC(ARG_INFO_PREFIX, name), (uint32_t) (sizeof(M_CONC(ARG_INFO_PREFIX, name))/sizeof(struct _zend_internal_arg_info)-1), M_CONC(M_CONC(php_, CLASS_NAME), _##name##_method.flags) },

/*
 * End function entry definition.
 */
#define REGISTER_FUNCTION_ENTRY_END() \
    PHP_FE_END \
    }


// Macros to provide quick and easy access to registering a php land class.

/*
 * Register a class to the zend engine with method definitions.
 */
#define REGISTER_CLASS_SET_ENTRY_EX(name, method_handlers) \
    M_CONC(CLASS_NAME, _entry) = register_zend_class<CLASS_TYPE>(name, php_##method_handlers)

/*
 * Register a class to the zend engine.
 */
#define REGISTER_CLASS_SET_ENTRY(name) \
    M_CONC(CLASS_NAME, _entry) = register_zend_class<CLASS_TYPE>(name, php_methods)

/*
 * Register a class to the zend engine with no methods.
 */
#define REGISTER_CLASS(name) \
    REGISTER_FUNCTION_ENTRY_START() \
    REGISTER_FUNCTION_ENTRY_END(); \
    REGISTER_CLASS_SET_ENTRY(name)

/*
 * Register a class to the zend engine with method definitions and object handlers.
 */
#define REGISTER_CLASS_WITH_HANDLERS_SET_ENTRY_EX(name, method_handlers) \
    M_CONC(CLASS_NAME, _entry) = register_zend_class<CLASS_TYPE>(&M_CONC(php_, M_CONC(CLASS_NAME, _handlers)), name, php_##method_handlers, M_CONC(M_CONC(allocate_, CLASS_NAME), _zend_obj))

/*
 * Register a class to the zend engine with object handlers.
 */
#define REGISTER_CLASS_WITH_HANDLERS_SET_ENTRY(name) \
    M_CONC(CLASS_NAME, _entry) = register_zend_class<CLASS_TYPE>(&M_CONC(php_, M_CONC(CLASS_NAME, _handlers)), name, php_methods, M_CONC(M_CONC(allocate_, CLASS_NAME), _zend_obj))

/*
 * Register a class to the zend engine with object handlers and no methods.
 */
#define REGISTER_CLASS_WITH_HANDLERS(name) \
    REGISTER_FUNCTION_ENTRY_START() \
    REGISTER_FUNCTION_ENTRY_END(); \
    REGISTER_CLASS_WITH_HANDLERS_SET_ENTRY(name)


//Helper macros for defining class constants.

#define REGISTER_CLASS_CONST_LONG(name, value) \
	zend_declare_class_constant_long(M_CONC(CLASS_NAME, _entry), name, sizeof(name)-1, (zend_long)value)

/*
 * Defines a php land class constant array of longs from a C-style integer array.
 */
#define REGISTER_CLASS_CONST_LONG_ARRAY(name, value) do { \
    zval arr; \
    array_init(&arr); \
    for(int i : value) { \
        zval val; \
        ZVAL_LONG(&val, i); \
        zend_hash_next_index_insert(Z_ARR_P(&arr), &val); \
    } \
    zend_declare_class_constant(M_CONC(CLASS_NAME, _entry), name, sizeof(name)-1, &arr); \
    } while (0)



// Helper macros for defining and updating class properties.

/*
 * Define a php class property with a default value.
 */
#define REGISTER_CLASS_PROPERTY_EX(name, acc_flags, default_value) \
    zend_declare_property(M_CONC(CLASS_NAME, _entry), name, sizeof(name)-1, default_value, acc_flags)

/*
* Define a php class property.
*/
#define REGISTER_CLASS_PROPERTY(name, acc_flags) do { \
    zval temp; \
    ZVAL_NULL(&temp); \
    REGISTER_CLASS_PROPERTY_EX(name, acc_flags, &temp); \
    }while(0)

/*
 * Define a php class property of type float with a default value.
 */
#define REGISTER_CLASS_PROPERTY_DOUBLE_EX(name, acc_flags, default_value) \
    zend_declare_property_double(M_CONC(CLASS_NAME, _entry), name, sizeof(name)-1, default_value, acc_flags)

/*
 * Define a php class property of type float.
 */
#define REGISTER_CLASS_PROPERTY_DOUBLE(name, acc_flags) \
    REGISTER_CLASS_PROPERTY_DOUBLE_EX(name, acc_flags, NULL)

/*
* Define a php class property of type long with a default value.
*/
#define REGISTER_CLASS_PROPERTY_LONG_EX(name, acc_flags, default_value) \
    zend_declare_property_long(M_CONC(CLASS_NAME, _entry), name, sizeof(name)-1, default_value, acc_flags)

/*
* Define a php class property of type long.
*/
#define REGISTER_CLASS_PROPERTY_LONG(name, acc_flags) \
    REGISTER_CLASS_PROPERTY_LONG_EX(name, acc_flags, NULL)

/*
 * Update a php class property.
 */
#define UPDATE_CLASS_PROPERTY_OTHER(obj, name, new_value) \
    zend_update_property(M_CONC(CLASS_NAME, _entry), obj, name, sizeof(name)-1, new_value)

/*
 * Update a php class property on the current object.
 */
#define UPDATE_CLASS_PROPERTY(name, new_value) \
    UPDATE_CLASS_PROPERTY_OTHER(getThis(), name, new_value)

/*
 * Update a php class property of type float.
 */
#define UPDATE_CLASS_PROPERTY_DOUBLE_OTHER(obj, name, new_value) \
    zend_update_property_double(M_CONC(CLASS_NAME, _entry), obj, name, sizeof(name)-1, new_value)

/*
* Update a php class property on the current object of type float.
*/
#define UPDATE_CLASS_PROPERTY_DOUBLE(name, new_value) \
    UPDATE_CLASS_PROPERTY_DOUBLE_OTHER(getThis(), name, new_value)

/*
* Update a php class property of type long.
*/
#define UPDATE_CLASS_PROPERTY_LONG_OTHER(obj, name, new_value) \
    zend_update_property_long(M_CONC(CLASS_NAME, _entry), obj, name, sizeof(name)-1, new_value)

/*
* Update a php class property on the current object of type long.
*/
#define UPDATE_CLASS_PROPERTY_LONG(name, new_value) \
    UPDATE_CLASS_PROPERTY_LONG_OTHER(getThis(), name, new_value)


// Helper macros for dealing with C++ objects encapsulated in a class_obj struct.

/*
 * Retrieve a class_obj struct of a custom type from a zval pointer.
 */
#define FETCH_PHP_OBJECT_OTHER_EX(type, obj) \
    fetch_from_zend_object<type>(Z_OBJ_P(obj))

/*
 * Retrieve a class_obj struct from a zval pointer.
 */
#define FETCH_PHP_OBJECT_OTHER(obj) \
    FETCH_PHP_OBJECT_OTHER_EX(CLASS_TYPE, obj)

/*
 * Retrieve a class_obj struct from the current class.
 */
#define FETCH_PHP_OBJECT() \
    FETCH_PHP_OBJECT_OTHER(getThis())

/*
 * Retrieve a class_obj struct from the current class and store in the variable var.
 */
#define FETCH_PHP_OBJECT_VAR(var) \
    auto var = FETCH_PHP_OBJECT()

/*
 * Retrieve a class_obj struct from a zval pointer and store in the variable var.
 */
#define FETCH_PHP_OBJECT_VAR_OTHER(var, obj_p) \
    auto var = FETCH_PHP_OBJECT_OTHER(obj_p)

/*
 * Retrieve a class_obj struct of a custom type from a zval pointer and store in the variable var.
 */
#define FETCH_PHP_OBJECT_VAR_OTHER_EX(type, var, obj_p) \
    auto var = FETCH_PHP_OBJECT_OTHER_EX(type, obj_p)

/*
 * Retrieve a pointer to the C++ object from the current class.
 */
#define FETCH_PHP_OBJECT_CONTAINER() \
    fetch_from_zend_object<CLASS_TYPE>(Z_OBJ_P(getThis()))->container

/*
 * Retrieve a pointer to the C++ object of a custom type from a zval pointer.
 */
#define FETCH_PHP_OBJECT_CONTAINER_OTHER_EX(type, obj_p) \
    fetch_from_zend_object<type>(Z_OBJ_P(obj_p))->container

/*
 * Retrieve a pointer to the C++ object from a zval pointer.
 */
#define FETCH_PHP_OBJECT_CONTAINER_OTHER(obj_p) \
    FETCH_PHP_OBJECT_CONTAINER_OTHER_EX(CLASS_TYPE, obj_p)

/*
 * Retrieve a pointer to the C++ object from a zval pointer and store in the variable var.
 */
#define FETCH_PHP_OBJECT_CONTAINER_VAR_OTHER_EX(type, var, obj_p) \
    auto var = FETCH_PHP_OBJECT_CONTAINER_OTHER_EX(type, obj_p)

/*
 * Retrieve a pointer to the C++ object from a zval pointer and store in the variable var.
 */
#define FETCH_PHP_OBJECT_CONTAINER_VAR_OTHER(var, obj_p) \
    auto var = FETCH_PHP_OBJECT_CONTAINER_OTHER(obj_p)

/*
 * Retrieve a pointer to the C++ object from the current class and store in the variable var.
 */
#define FETCH_PHP_OBJECT_CONTAINER_VAR(var) \
    FETCH_PHP_OBJECT_CONTAINER_VAR_OTHER(var, getThis())

/*
 * Set the C++ object of a zval pointer.
 */
#define SET_PHP_OBJECT_CONTAINER_EX(value, obj) do { \
    FETCH_PHP_OBJECT_VAR_OTHER(temp_##type, obj); \
    temp_##type->container = value; \
    } while (0)

/*
 * Set the C++ object of the current object.
 */
#define SET_PHP_OBJECT_CONTAINER(value) \
    SET_PHP_OBJECT_CONTAINER_EX(value, getThis())

#endif //EXT_POCKETMINE_MATH_ZENDUTIL_H
