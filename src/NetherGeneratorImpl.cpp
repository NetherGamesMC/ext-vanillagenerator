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
  zval *palettedArray;
  zend_string *biomeArray;
  zend_long morton;
  ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 3, 3)
    Z_PARAM_ARRAY_EX(palettedArray, 1, 1)
    Z_PARAM_STR_EX(biomeArray, 1, 1)
    Z_PARAM_LONG(morton)
  ZEND_PARSE_PARAMETERS_END();

  if (ZSTR_LEN(biomeArray) != BiomeArray::DATA_SIZE) {
    zend_throw_exception_ex(spl_ce_InvalidArgumentException, 0, "Biome array is expected to be exactly %zu bytes, but got %zu bytes", BiomeArray::DATA_SIZE, ZSTR_LEN(biomeArray));
    RETURN_THROWS();
  }

  gsl::span<const uint8_t, BiomeArray::DATA_SIZE> span(reinterpret_cast<const uint8_t *>(ZSTR_VAL(biomeArray)), BiomeArray::DATA_SIZE);

  auto chunkManager = ChunkManager(Y_MIN, Y_MAX);
  auto storage = fetch_from_zend_object<nether_generator>(Z_OBJ_P(getThis()));

  zval new_class;
  zval *element;
  zend_string *key;
  zend_ulong hash;

  std::array<NormalBlockArrayContainer *, 16> blockContainers{};
  blockContainers.fill(nullptr);

  zend_array *hashTable = Z_ARRVAL_P(palettedArray);
  ZEND_HASH_FOREACH_KEY_VAL(hashTable, hash, key, element) {
    bool isNull = Z_TYPE_P(element) == IS_NULL;
    bool isObject = Z_TYPE_P(element) == IS_OBJECT;

    if ((!isNull && !isObject) || (isObject && !instanceof_function(Z_OBJCE_P(element), storage->paletted_block_entry_class))) {
      if (key == nullptr) {
        zend_type_error(R"(The array index in key %lld of %lld must be of type \pocketmine\world\format\PalettedBlockArray|null, %s given)", hash, morton, zend_zval_type_name(element));
      } else {
        zend_type_error(R"(The array index in key '%s' of %lld must be of type \pocketmine\world\format\PalettedBlockArray|null, %s given)", key->val, morton, zend_zval_type_name(element));
      }
      RETURN_THROWS();
    }

    try {
      if (!isNull) {
        auto object = fetch_from_zend_object<paletted_block_array_obj>(Z_OBJ_P(element));
        blockContainers.at(hash) = &object->container;
      } else {
        object_init_ex(&new_class, storage->paletted_block_entry_class);

        auto object = fetch_from_zend_object<paletted_block_array_obj>(Z_OBJ_P(&new_class));
        new (&object->container) NormalBlockArrayContainer((Block)0);

        blockContainers.at(hash) = &object->container;

        if (key) {
          zend_hash_update(hashTable, key, &new_class);
        } else {
          zend_hash_index_update(hashTable, hash, &new_class);
        }
      }
    } catch(std::out_of_range const& exc) {
      zend_throw_error(nullptr, "Array for PalettedBlockArray must have exactly 16 defined entries.");
      return;
    }
  } ZEND_HASH_FOREACH_END();

  auto biomeClass = BiomeArray(span);
  auto chunk = new Chunk(static_cast<int64_t>(morton), blockContainers, biomeClass);

  chunkManager.SetChunk(chunk->GetX(), chunk->GetZ(), chunk);

  auto generator = storage->overworldGenerator;

  try {
    generator->GenerateChunk(chunkManager, chunk->GetX(), chunk->GetZ());
  } catch (std::exception &error) {
    zend_throw_error(zend_ce_exception, "**INTERNAL GENERATOR ERROR** %s", error.what());
    RETURN_THROWS();
  }

  auto raw_array = chunk->GetBiomeArray().GetRawData();

  RETURN_STRINGL(reinterpret_cast<const char*>(raw_array.data()), raw_array.size_bytes());
}

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_NetherGenerator_populateChunk, 0, 4, IS_VOID, 0)
  ZEND_ARG_TYPE_INFO(1, palettedArray, IS_ARRAY, 0)
  ZEND_ARG_TYPE_INFO(0, biomeArray, IS_ARRAY, 0)
  ZEND_ARG_TYPE_INFO(1, dirtyFlags, IS_ARRAY, 0)
  ZEND_ARG_TYPE_INFO(0, morton, IS_LONG, 0)
ZEND_END_ARG_INFO()

PHP_METHOD (NetherGenerator, populateChunk) {
  zval *palettedArray;
  zval *biomeArray;
  zval *dirtyFlags;
  zend_long morton;
  ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 4, 4)
    Z_PARAM_ARRAY_EX(palettedArray, 1, 1)
    Z_PARAM_ARRAY_EX(biomeArray, 1, 1)
    Z_PARAM_ARRAY_EX(dirtyFlags, 1, 1)
    Z_PARAM_LONG(morton)
  ZEND_PARSE_PARAMETERS_END();

  auto chunkManager = ChunkManager(Y_MIN, Y_MAX);
  auto storage = fetch_from_zend_object<nether_generator>(Z_OBJ_P(getThis()));

  int64_t chunkX, chunkZ;
  morton2d_decode(morton, chunkX, chunkZ);

  // First multidimensional array variables.
  zval *parent_element;
  zend_string *parent_key;
  zend_ulong parent_hash;

  // The real deal we are going to handle variables.
  zval new_class;
  zval *element;
  zend_string *key;
  zend_ulong hash;
  ZEND_HASH_FOREACH_KEY_VAL(Z_ARRVAL_P(palettedArray), parent_hash, parent_key, parent_element) {
    if (parent_key) {
      zend_type_error("The array keys must be an integer, the keys must be a valid chunk coordinates for its SubChunks");
      RETURN_THROWS();
    } else if (Z_TYPE_P(parent_element) != IS_ARRAY) {
      zend_type_error("The array value of index %lld must be an array.", parent_hash);
      RETURN_THROWS();
    } else {
      std::array<NormalBlockArrayContainer *, 16> blockContainers{};
      blockContainers.fill(nullptr);

      zend_array *hashTable = Z_ARRVAL_P(parent_element);
      ZEND_HASH_FOREACH_KEY_VAL(hashTable, hash, key, element) {
        bool isNull = Z_TYPE_P(element) == IS_NULL;
        bool isObject = Z_TYPE_P(element) == IS_OBJECT;

        if ((!isNull && !isObject) || (isObject && !instanceof_function(Z_OBJCE_P(element), storage->paletted_block_entry_class))) {
          if (key == nullptr) {
            zend_type_error(R"(The array index in key %lld of %lld must be of type \pocketmine\world\format\PalettedBlockArray|null, %s given)", hash, parent_hash, zend_zval_type_name(element));
          } else {
            zend_type_error(R"(The array index in key '%s' of %lld must be of type \pocketmine\world\format\PalettedBlockArray|null, %s given)", key->val, parent_hash, zend_zval_type_name(element));
          }
          RETURN_THROWS();
        }

        try {
          if (!isNull) {
            auto object = fetch_from_zend_object<paletted_block_array_obj>(Z_OBJ_P(element));
            blockContainers.at(hash) = &object->container;
          } else {
            object_init_ex(&new_class, storage->paletted_block_entry_class);

            auto object = fetch_from_zend_object<paletted_block_array_obj>(Z_OBJ_P(&new_class));
            new (&object->container) NormalBlockArrayContainer((Block)0);

            blockContainers.at(hash) = &object->container;

            if (key) {
              zend_hash_update(hashTable, key, &new_class);
            } else {
              zend_hash_index_update(hashTable, hash, &new_class);
            }
          }
        } catch(std::out_of_range const& exc) {
          zend_throw_error(nullptr, "Array for PalettedBlockArray must have exactly 16 defined entries.");
          return;
        }
      } ZEND_HASH_FOREACH_END();

      zval* biome_array = zend_hash_index_find(Z_ARRVAL_P(biomeArray), parent_hash);
      zval* hash_index = zend_hash_index_find(Z_ARRVAL_P(dirtyFlags), parent_hash);

      if (biome_array == nullptr) {
        zend_throw_error(nullptr, "Chunk for hash %lld does not present in biome array.", parent_hash);
        RETURN_THROWS();
      } else if (hash_index == nullptr) {
        zend_throw_error(nullptr, "Dirty flags for hash %lld does not present in dirty flags array.", parent_hash);
        RETURN_THROWS();
      } else if (Z_TYPE_P(biome_array) != IS_STRING) {
        zend_throw_exception_ex(spl_ce_InvalidArgumentException, 0, "Biome array must be a string in index %lld, %s given", parent_hash, zend_zval_type_name(biome_array));
        RETURN_THROWS();
      } else if (Z_STRLEN_P(biome_array) != BiomeArray::DATA_SIZE) {
        zend_throw_exception_ex(spl_ce_InvalidArgumentException, 0, "Biome array is expected to be exactly %zu bytes, but got %zu bytes", BiomeArray::DATA_SIZE, Z_STRLEN_P(biome_array));
        RETURN_THROWS();
      } else if (Z_TYPE_P(hash_index) != IS_FALSE && Z_TYPE_P(hash_index) != IS_TRUE && Z_TYPE_P(hash_index) != _IS_BOOL) {
        zend_throw_exception_ex(spl_ce_InvalidArgumentException, 0, "Dirty flags must be a boolean in index %lld, %s given", parent_hash, zend_zval_type_name(hash_index));
        RETURN_THROWS();
      }

      gsl::span<const uint_fast8_t, BiomeArray::DATA_SIZE> span(reinterpret_cast<const uint_fast8_t *>(Z_STR_P(biome_array)), BiomeArray::DATA_SIZE);

      auto biomeClass = BiomeArray(span);
      auto chunk = new Chunk(static_cast<int64_t>(parent_hash), blockContainers, biomeClass);
      chunk->SetDirty(Z_TYPE_P(hash_index) == IS_TRUE);

      chunkManager.SetChunk(chunk->GetX(), chunk->GetZ(), chunk);
    }
  } ZEND_HASH_FOREACH_END();

  auto generator = storage->overworldGenerator;

  try {
    generator->PopulateChunk(chunkManager, static_cast<int_fast32_t>(chunkX), static_cast<int_fast32_t>(chunkZ));
  } catch (std::exception &error) {
    zend_throw_error(zend_ce_exception, "**INTERNAL GENERATOR ERROR** %s", error.what());
    RETURN_THROWS();
  }

  zval boolObject;
  for (auto x : chunkManager.GetChunks()) {
    ZVAL_BOOL(&boolObject, x.second->IsDirty());

    zend_hash_index_update(Z_ARRVAL_P(dirtyFlags), static_cast<zend_long>(x.first), &boolObject);
  }
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