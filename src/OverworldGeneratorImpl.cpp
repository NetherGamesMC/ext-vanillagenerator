#include <PhpPalettedBlockArrayObj.h>

#include <lib/ZendUtil.h>
#include <lib/MortonHelper.h>
#include <lib/chunk/ChunkManager.h>
#include <lib/biomes/BiomeClimate.h>

#include "OverworldGeneratorImpl.h"

extern "C" {
  #include <php.h>
  #include <zend_exceptions.h>
  #include <ext/spl/spl_exceptions.h>
}

PHP_METHOD (OverworldGenerator, __construct) {
  zend_long seed;
  zend_bool isUHC = false;

  ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 2)
    Z_PARAM_LONG(seed)
    Z_PARAM_OPTIONAL
    Z_PARAM_BOOL(isUHC)
  ZEND_PARSE_PARAMETERS_END();

  // Attempt to initialize PalettedBlockArray class entry, if it does not exist,
  // it simply means that the server has no ext-chunkutils2 installed.
  auto object = fetch_from_zend_object<overworld_generator>(Z_OBJ_P(getThis()));

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

  object->overworldGenerator = new OverworldGenerator(static_cast<int_fast64_t>(seed), isUHC);
}

/**
 *  @brief Generates terrains for the given chunk
 *
 *  It is important to understand that this generated terrain is not the same as terrain population,
 *  terrain generation only takes 1 chunk area while terrain population can take up to 9 chunks (8 neighbouring
 *  chunks).
 */
PHP_METHOD (OverworldGenerator, generateChunk) {
  zval *blockArray;
  zval *biomeArray;
  zend_long morton;

  ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 3, 3)
    Z_PARAM_ARRAY_EX(blockArray, 1, 1)
    Z_PARAM_ARRAY_EX(biomeArray, 1, 1)
    Z_PARAM_LONG(morton)
  ZEND_PARSE_PARAMETERS_END();

  auto chunkManager = ChunkManager(Chunk::Y_MIN, Chunk::Y_MAX);
  auto storage = fetch_from_zend_object<overworld_generator>(Z_OBJ_P(getThis()));

  zval *element;
  zend_ulong hash;
  zend_string *key;

  std::array<NormalBlockArrayContainer *, Chunk::MAX_SUBCHUNKS> blockContainers{};
  std::array<NormalBlockArrayContainer *, Chunk::MAX_SUBCHUNKS> biomeContainers{};

  int id = 0;
  ZEND_HASH_FOREACH_KEY_VAL(Z_ARRVAL_P(blockArray), hash, key, element) {
    // We only need an object of "PalettedBlockArray"
    if (Z_TYPE_P(element) == IS_OBJECT && !instanceof_function(Z_OBJCE_P(element), storage->paletted_block_entry_class)) {
      zend_type_error("The array objects for $palettedArray must be an instanceof PalettedBlockArray");
      RETURN_THROWS();
    }

    try {
      auto object = fetch_from_zend_object<paletted_block_array_obj>(Z_OBJ_P(element));
      blockContainers.at(id++) = &object->container;
    } catch(std::out_of_range const& exc) {
      zend_throw_error(nullptr, "The array objects for $palettedArray must contain exactly 24 PalettedBlockArray");
      return;
    }
  } ZEND_HASH_FOREACH_END();

  id = 0;
  ZEND_HASH_FOREACH_KEY_VAL(Z_ARRVAL_P(biomeArray), hash, key, element) {
    // We only need an object of "PalettedBlockArray"
    if (Z_TYPE_P(element) == IS_OBJECT && !instanceof_function(Z_OBJCE_P(element), storage->paletted_block_entry_class)) {
      zend_type_error("The array objects for $biomeArray must be an instanceof PalettedBlockArray");
      RETURN_THROWS();
    }

    try {
      auto object = fetch_from_zend_object<paletted_block_array_obj>(Z_OBJ_P(element));
      biomeContainers.at(id++) = &object->container;
    } catch(std::out_of_range const& exc) {
      zend_throw_error(nullptr, "The array objects for $biomeArray must contain exactly 24 PalettedBlockArray");
      return;
    }
  } ZEND_HASH_FOREACH_END();

  auto biome = MCBiomeArray(biomeContainers);
  auto chunk = Chunk(morton, blockContainers, biome);

  chunkManager.SetChunk(chunk.GetX(), chunk.GetZ(), &chunk);

  try {
    storage->overworldGenerator->GenerateChunk(chunkManager, chunk.GetX(), chunk.GetZ());
  } catch (std::exception &error) {
    zend_throw_error(zend_ce_exception, "**INTERNAL GENERATOR ERROR** %s", error.what());
    RETURN_THROWS();
  }
}

PHP_METHOD (OverworldGenerator, populateChunk) {
  // TODO: Handle chunk population
}

PHP_METHOD (OverworldGenerator, registerBlock) {
  using namespace blocks;

  zend_long blockStateId;
  zend_long meta;
  zend_long blockMetadata;

  ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 3, 3)
    Z_PARAM_LONG(blockStateId)
    Z_PARAM_LONG(meta)
    Z_PARAM_LONG(blockMetadata)
  ZEND_PARSE_PARAMETERS_END();

  MCBlock::RegisterBlock(blockStateId, meta, blockMetadata);
}

ZEND_BEGIN_ARG_INFO_EX(arginfo_OverworldGenerator___construct, 0, 0, 1)
  ZEND_ARG_TYPE_INFO(0, seed, IS_LONG, 0)
  ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, isUHC, _IS_BOOL, 0, "false")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_OverworldGenerator_generateChunk, 0, 3, IS_VOID, 0)
  ZEND_ARG_TYPE_INFO(1, blockArray, IS_ARRAY, 0)
  ZEND_ARG_TYPE_INFO(1, biomeArray, IS_ARRAY, 0)
  ZEND_ARG_TYPE_INFO(0, morton, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_OverworldGenerator_populateChunk, 0, 4, IS_VOID, 0)
  ZEND_ARG_TYPE_INFO(1, palettedArray, IS_ARRAY, 0)
  ZEND_ARG_TYPE_INFO(0, biomeArray, IS_ARRAY, 0)
  ZEND_ARG_TYPE_INFO(1, dirtyFlags, IS_ARRAY, 0)
  ZEND_ARG_TYPE_INFO(0, morton, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_OverworldGenerator_registerBlock, 0, 3, IS_VOID, 0)
  ZEND_ARG_TYPE_INFO(0, blockStateId, IS_LONG, 0)
  ZEND_ARG_TYPE_INFO(0, id, IS_LONG, 0)
  ZEND_ARG_TYPE_INFO(0, blockMetadata, IS_LONG, 0)
ZEND_END_ARG_INFO()

zend_function_entry overworld_methods[] = {
  PHP_ME(OverworldGenerator, __construct, arginfo_OverworldGenerator___construct, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
  PHP_ME(OverworldGenerator, populateChunk, arginfo_OverworldGenerator_populateChunk, ZEND_ACC_PUBLIC)
  PHP_ME(OverworldGenerator, generateChunk, arginfo_OverworldGenerator_generateChunk, ZEND_ACC_PUBLIC)
  PHP_ME(OverworldGenerator, registerBlock, arginfo_OverworldGenerator_registerBlock, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
  PHP_FE_END
};

static zend_object_handlers overworld_populator_handlers;

static zend_object *generator_new(zend_class_entry *class_type) {
  auto object = alloc_custom_zend_object<overworld_generator>(class_type, &overworld_populator_handlers);

  return &object->std;
}

static void generator_free(zend_object *obj) {
  auto object = fetch_from_zend_object<overworld_generator>(obj);
  delete object->overworldGenerator;

  zend_object_std_dtor(obj);
}

void register_overworld_generator() {
  memcpy(&overworld_populator_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
  overworld_populator_handlers.offset = XtOffsetOf(overworld_generator, std);
  overworld_populator_handlers.free_obj = generator_free;

  zend_class_entry cle;
  INIT_CLASS_ENTRY(cle, "OverworldGenerator", overworld_methods);
  cle.create_object = generator_new;
  zend_register_internal_class(&cle);
}