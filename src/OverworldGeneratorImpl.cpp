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

  try {
    object->overworldGenerator = new OverworldGenerator(seed, isUHC);
  } catch (std::exception& e) {
    zend_throw_exception_ex(spl_ce_RuntimeException, 0, "%s", e.what());
  }
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

  auto storage = fetch_from_zend_object<overworld_generator>(Z_OBJ_P(getThis()));

  zval *element;

  BlockContainer blockContainers{};
  BlockContainer biomeContainers{};

  int id = 0;
  ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(blockArray), element) {
    // We only need an object of "PalettedBlockArray"
    if (Z_TYPE_P(element) == IS_OBJECT && !instanceof_function(Z_OBJCE_P(element), storage->paletted_block_entry_class)) {
      zend_type_error("The array objects for $palettedArray must be an instanceof PalettedBlockArray");
      RETURN_THROWS();
    }

    try {
      auto object = fetch_from_zend_object<paletted_block_array_obj>(Z_OBJ_P(element));
      blockContainers.at(id++) = &object->container;
    } catch(std::out_of_range const&) {
      zend_throw_error(nullptr, "The array objects for $palettedArray must contain exactly 24 PalettedBlockArray");
      return;
    }
  } ZEND_HASH_FOREACH_END();

  id = 0;
  ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(biomeArray), element) {
    // We only need an object of "PalettedBlockArray"
    if (Z_TYPE_P(element) != IS_OBJECT || !instanceof_function(Z_OBJCE_P(element), storage->paletted_block_entry_class)) {
      zend_type_error("The array objects for $biomeArray must be an instanceof PalettedBlockArray");
      RETURN_THROWS();
    }

    try {
      auto object = fetch_from_zend_object<paletted_block_array_obj>(Z_OBJ_P(element));
      biomeContainers.at(id++) = &object->container;
    } catch(std::out_of_range const&) {
      zend_throw_error(nullptr, "The array objects for $biomeArray must contain exactly 24 PalettedBlockArray");
      return;
    }
  } ZEND_HASH_FOREACH_END();

  auto biome = MCBiomeArray(biomeContainers);
  auto chunk = Chunk(morton, blockContainers, biome);

  auto chunkManager = ChunkManager(Chunk::Y_MIN, Chunk::Y_MAX);
  chunkManager.SetChunk(chunk.GetX(), chunk.GetZ(), &chunk);

  try {
    storage->overworldGenerator->GenerateChunk(chunkManager, chunk.GetX(), chunk.GetZ());
  } catch (std::exception &error) {
    zend_throw_error(zend_ce_exception, "**INTERNAL GENERATOR ERROR** %s", error.what());
    RETURN_THROWS();
  }
}

PHP_METHOD (OverworldGenerator, populateChunk) {
  zval *blockEntries;
  zend_long morton;

  ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 2, 2)
    Z_PARAM_ARRAY_EX(blockEntries, 1, 1)
    Z_PARAM_LONG(morton)
  ZEND_PARSE_PARAMETERS_END();

  auto chunkManager = ChunkManager(Chunk::Y_MIN, Chunk::Y_MAX);
  auto storage = fetch_from_zend_object<overworld_generator>(Z_OBJ_P(getThis()));

  // hash -> BlockContainer
  std::map<uint_fast64_t, BlockContainer> blockParsedEntries{};
  std::map<uint_fast64_t, BlockContainer> biomeParsedEntries{};
  std::map<uint_fast64_t, bool> dirtyParsedEntries{};

  // We need to allocate the map of objects so that they are allocated inside the stack,
  // later they will be deallocated after the method goes out of scope.
  // Maybe we can improve this by having these objects initialized first then reuse in a thread local heap?
  std::map<uint_fast64_t, Chunk> blocksEntries{};
  std::map<uint_fast64_t, MCBiomeArray> biomesEntries{};

  // Parse objects from a multidimensional blockEntries array.

  zval *parent_element;
  uint_fast64_t parent_hash;

  zval *element;
  zend_string *chunk_key;
  zend_ulong chunk_hash;
  ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(blockEntries), parent_element) {
    if (Z_TYPE_P(parent_element) != IS_ARRAY) {
      zend_type_error("The array key parameter does not satisfy the required elements for a chunk population.");
      RETURN_THROWS();
    }

    zval *actual_hash = zend_hash_index_find(Z_ARRVAL_P(parent_element), 0);
    zval *actual_elements = zend_hash_index_find(Z_ARRVAL_P(parent_element), 1);
    zval *is_dirty = zend_hash_index_find(Z_ARRVAL_P(parent_element), 2);
    if (!actual_hash || !actual_elements || !is_dirty || Z_TYPE_P(actual_hash) != IS_LONG || Z_TYPE_P(actual_elements) != IS_ARRAY || (Z_TYPE_P(is_dirty) != IS_FALSE && Z_TYPE_P(is_dirty) != IS_TRUE && Z_TYPE_P(is_dirty) != _IS_BOOL)) {
      zend_type_error("The first array value parameter requires 3 additional arrays.");
      RETURN_THROWS();
    }

    parent_hash = (uint_fast64_t) Z_LVAL_P(actual_hash);

    zval *blocks = zend_hash_index_find(Z_ARRVAL_P(actual_elements), 0);
    zval *biomes = zend_hash_index_find(Z_ARRVAL_P(actual_elements), 1);
    if (!blocks || !biomes) {
      zend_type_error("The second array value parameter requires 2 additional arrays.");
      RETURN_THROWS();
    }

    blockParsedEntries.insert({parent_hash, {}});
    biomeParsedEntries.insert({parent_hash, {}});
    dirtyParsedEntries.insert({parent_hash, Z_TYPE_P(is_dirty) == IS_TRUE});

    // The block array.
    ZEND_HASH_FOREACH_KEY_VAL(Z_ARRVAL_P(blocks), chunk_hash, chunk_key, element) {
      if (Z_TYPE_P(element) != IS_OBJECT || !instanceof_function(Z_OBJCE_P(element), storage->paletted_block_entry_class)) {
        zend_type_error("The first array of objects for chunk %llu must be an instanceof PalettedBlockArray.", parent_hash);
        RETURN_THROWS();
      }

      auto object = fetch_from_zend_object<paletted_block_array_obj>(Z_OBJ_P(element));
      blockParsedEntries.at(parent_hash).at(chunk_hash) = &object->container;
    } ZEND_HASH_FOREACH_END();

    // The biome array.
    ZEND_HASH_FOREACH_KEY_VAL(Z_ARRVAL_P(biomes), chunk_hash, chunk_key, element) {
      if (Z_TYPE_P(element) != IS_OBJECT || !instanceof_function(Z_OBJCE_P(element), storage->paletted_block_entry_class)) {
        zend_type_error("The second array of objects for chunk %llu must be an instanceof PalettedBlockArray.", parent_hash);
        RETURN_THROWS();
      }

      auto object = fetch_from_zend_object<paletted_block_array_obj>(Z_OBJ_P(element));
      biomeParsedEntries.at(parent_hash).at(chunk_hash) = &object->container;
    } ZEND_HASH_FOREACH_END();

    biomesEntries.insert({parent_hash, biomeParsedEntries.at(parent_hash)});
    blocksEntries.insert({parent_hash, {parent_hash, blockParsedEntries.at(parent_hash), biomesEntries.at(parent_hash)}});

    auto &chunk = blocksEntries.at(parent_hash);
    chunk.SetDirty(dirtyParsedEntries.at(parent_hash));

    chunkManager.SetChunk(chunk.GetX(), chunk.GetZ(), &chunk);
  } ZEND_HASH_FOREACH_END();

  try {
    auto generator = storage->overworldGenerator;

    int_fast32_t chunkX, chunkZ;
    morton2d_decode(morton, chunkX, chunkZ);

    generator->PopulateChunk(chunkManager, chunkX, chunkZ);
  } catch (std::exception &error) {
    zend_throw_error(zend_ce_exception, "**INTERNAL GENERATOR ERROR** %s", error.what());
    RETURN_THROWS();
  }

  array_init(return_value);
  const auto &chunks = chunkManager.GetChunks();

  ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(blockEntries), parent_element) {
    // Find the block hash, then cross-reference it with the chunk manager.
    zval *actual_hash = zend_hash_index_find(Z_ARRVAL_P(parent_element), 0);

    auto chunk_hash_result =  Z_LVAL_P(actual_hash);
    auto mask = chunks.find((uint_fast64_t) chunk_hash_result);
    if (mask == chunks.end()) {
      zend_throw_error(zend_ce_exception, "Chunk hash of %llu was not found in the array, this should never happen.", parent_hash);
      RETURN_THROWS();
    }

    add_index_bool(return_value, chunk_hash_result, chunks.at(chunk_hash_result)->IsDirty() ? 1 : 0);
  } ZEND_HASH_FOREACH_END();
}

PHP_METHOD (OverworldGenerator, registerBlock) {
  zend_long blockStateId;
  zend_long meta;
  zend_long blockMetadata;

  ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 3, 3)
    Z_PARAM_LONG(blockStateId)
    Z_PARAM_LONG(meta)
    Z_PARAM_LONG(blockMetadata)
  ZEND_PARSE_PARAMETERS_END();

  try {
    MCBlock::RegisterBlock(blockStateId, meta, blockMetadata);
  } catch (std::exception &error) {
    zend_throw_error(zend_ce_exception, "Cannot register block: %s", error.what());
    RETURN_THROWS();
  }
}

PHP_METHOD (OverworldGenerator, registerMask) {
  zend_long blockTypeId;
  zend_long blockMask;

  ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 2, 2)
    Z_PARAM_LONG(blockTypeId)
    Z_PARAM_LONG(blockMask)
  ZEND_PARSE_PARAMETERS_END();

  try {
    MCBlock::RegisterMask(blockTypeId, blockMask);
  } catch (std::exception &error) {
    zend_throw_error(zend_ce_exception, "Cannot register mask: %s", error.what());
    RETURN_THROWS();
  }
}

ZEND_BEGIN_ARG_INFO_EX(arginfo_OverworldGenerator___construct, 0, 0, 1)
  ZEND_ARG_TYPE_INFO(0, seed, IS_LONG, 0)
  ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, isUHC, _IS_BOOL, 0, "false")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_OverworldGenerator_generateChunk, 0, 3, IS_VOID, 0)
  ZEND_ARG_TYPE_INFO(1, blockArray, IS_ARRAY, 1)
  ZEND_ARG_TYPE_INFO(1, biomeArray, IS_ARRAY, 1)
  ZEND_ARG_TYPE_INFO(0, populatePosition, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_OverworldGenerator_populateChunk, 0, 2, IS_ARRAY, 0)
  ZEND_ARG_TYPE_INFO(1, blockEntries, IS_ARRAY, 1)
  ZEND_ARG_TYPE_INFO(0, populatePosition, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_OverworldGenerator_registerBlock, 0, 3, IS_VOID, 0)
  ZEND_ARG_TYPE_INFO(0, blockStateId, IS_LONG, 0)
  ZEND_ARG_TYPE_INFO(0, id, IS_LONG, 0)
  ZEND_ARG_TYPE_INFO(0, blockMetadata, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_OverworldGenerator_registerMask, 0, 2, IS_VOID, 0)
  ZEND_ARG_TYPE_INFO(0, blockTypeId, IS_LONG, 0)
  ZEND_ARG_TYPE_INFO(0, blockMask, IS_LONG, 0)
ZEND_END_ARG_INFO()

zend_function_entry overworld_methods[] = {
  PHP_ME(OverworldGenerator, __construct, arginfo_OverworldGenerator___construct, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
  PHP_ME(OverworldGenerator, populateChunk, arginfo_OverworldGenerator_populateChunk, ZEND_ACC_PUBLIC)
  PHP_ME(OverworldGenerator, generateChunk, arginfo_OverworldGenerator_generateChunk, ZEND_ACC_PUBLIC)
  PHP_ME(OverworldGenerator, registerBlock, arginfo_OverworldGenerator_registerBlock, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
  PHP_ME(OverworldGenerator, registerMask, arginfo_OverworldGenerator_registerMask, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
  PHP_FE_END
};

static zend_object_handlers overworld_generator_handlers;

static zend_object *generator_new(zend_class_entry *class_type) {
  auto object = alloc_custom_zend_object<overworld_generator>(class_type, &overworld_generator_handlers);

  return &object->std;
}

static void generator_free(zend_object *obj) {
  auto object = fetch_from_zend_object<overworld_generator>(obj);
  delete object->overworldGenerator;

  zend_object_std_dtor(obj);
}

void register_overworld_generator() {
  memcpy(&overworld_generator_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
  overworld_generator_handlers.offset = XtOffsetOf(overworld_generator, std);
  overworld_generator_handlers.free_obj = generator_free;

  zend_class_entry cle;
  INIT_CLASS_ENTRY(cle, "OverworldGenerator", overworld_methods);
  cle.create_object = generator_new;
  zend_register_internal_class(&cle);
}