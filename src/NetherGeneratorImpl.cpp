#include <PhpPalettedBlockArrayObj.h>

#include <lib/ZendUtil.h>
#include <lib/MortonHelper.h>
#include <lib/chunk/ChunkManager.h>
#include <lib/biomes/BiomeClimate.h>

#include "NetherGeneratorImpl.h"

extern "C" {
  #include <php.h>
  #include <zend_exceptions.h>
  #include <ext/spl/spl_exceptions.h>
}

static zend_object_handlers overworld_populator_handlers;

static zend_object *generator_new(zend_class_entry *class_type) {
  auto object = alloc_custom_zend_object<nether_generator>(class_type, &overworld_populator_handlers);

  return &object->std;
}

static void generator_free(zend_object *obj) {
  auto object = fetch_from_zend_object<nether_generator>(obj);
  delete object->overworldGenerator;

  zend_object_std_dtor(obj);
}

ZEND_BEGIN_ARG_INFO_EX(arginfo_NetherGenerator___construct, 0, 0, 1)
  ZEND_ARG_TYPE_INFO(0, seed, IS_LONG, 0)
ZEND_END_ARG_INFO()

PHP_METHOD (NetherGenerator, __construct) {
  zend_long seed;
  ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 2)
    Z_PARAM_LONG(seed)
  ZEND_PARSE_PARAMETERS_END();

  // Attempt to initialize PalettedBlockArray class entry, if it does not exist,
  // it simply means that the server has no ext-chunkutils2 installed.
  auto object = fetch_from_zend_object<nether_generator>(Z_OBJ_P(getThis()));

  zend_string *className = zend_string_init(ZEND_STRL(R"(\pocketmine\world\format\PalettedBlockArray)"), true);

  zend_class_entry *ce;
  if ((ce = zend_lookup_class(className)) != nullptr) {
    object->paletted_block_entry_class = ce;
  } else {
    zend_string_release(className);

    zend_throw_error(nullptr, "ext-chunkutils2 is required for ext-vanillagenerator to function correctly.");
    RETURN_THROWS();
  }

  zend_string_release(className);

  object->overworldGenerator = new NetherGenerator(static_cast<int_fast64_t>(seed));
}

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_NetherGenerator_generateChunk, 0, 3, IS_STRING, 0)
  ZEND_ARG_TYPE_INFO(1, palettedArray, IS_ARRAY, 0)
  ZEND_ARG_TYPE_INFO(0, biomeArray, IS_STRING, 0)
  ZEND_ARG_TYPE_INFO(0, morton, IS_LONG, 0)
ZEND_END_ARG_INFO()

PHP_METHOD (NetherGenerator, generateChunk) {
}

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_NetherGenerator_populateChunk, 0, 4, IS_VOID, 0)
  ZEND_ARG_TYPE_INFO(1, palettedArray, IS_ARRAY, 0)
  ZEND_ARG_TYPE_INFO(0, biomeArray, IS_ARRAY, 0)
  ZEND_ARG_TYPE_INFO(1, dirtyFlags, IS_ARRAY, 0)
  ZEND_ARG_TYPE_INFO(0, morton, IS_LONG, 0)
ZEND_END_ARG_INFO()

PHP_METHOD (NetherGenerator, populateChunk) {
}

zend_function_entry nether_methods[] = {
    PHP_ME(NetherGenerator, __construct, arginfo_NetherGenerator___construct, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
    PHP_ME(NetherGenerator, populateChunk, arginfo_NetherGenerator_populateChunk, ZEND_ACC_PUBLIC)
    PHP_ME(NetherGenerator, generateChunk, arginfo_NetherGenerator_generateChunk, ZEND_ACC_PUBLIC)
    PHP_FE_END
};

void register_nether_generator() {
  memcpy(&overworld_populator_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
  overworld_populator_handlers.offset = XtOffsetOf(nether_generator, std);
  overworld_populator_handlers.free_obj = generator_free;

  zend_class_entry cle;
  INIT_CLASS_ENTRY(cle, "NetherGenerator", nether_methods);
  cle.create_object = generator_new;
  zend_register_internal_class(&cle);
}