#include <PhpPalettedBlockArrayObj.h>

#include <lib/ZendUtil.h>
#include <lib/MortonHelper.h>
#include <lib/pocketmine/Logic.h>
#include <lib/chunk/SimpleChunkManager.h>
#include <lib/vanilla/Biome.h>

#include "RandomImpl.h"
#include "OverworldChunkPopulator.h"

extern "C" {
    #include <php.h>
    #include <zend_exceptions.h>
    #include <ext/spl/spl_exceptions.h>
}

static zend_object_handlers overworld_populator_handlers;

static zend_object *populator_new(zend_class_entry *class_type) {
    auto object = alloc_custom_zend_object<overworld_populators>(class_type, &overworld_populator_handlers);

    return &object->std;
}

static void populator_free(zend_object *obj) {
    auto object = fetch_from_zend_object<overworld_populators>(obj);
    object->overworldPopulator.destroy();

    zend_object_std_dtor(obj);
}

ZEND_BEGIN_ARG_INFO_EX(arginfo_OverworldChunkPopulator___construct, 0, 0, 0)
ZEND_END_ARG_INFO()

PHP_METHOD (OverworldChunkPopulator, __construct) {
    // Attempt to initialize PalettedBlockArray class entry, if it does not exists,
    // it simply means that the server has no ext-chunkutils2 installed.
    auto object = fetch_from_zend_object<overworld_populators>(Z_OBJ_P(getThis()));

    zend_string *className = zend_string_init(ZEND_STRL(R"(\pocketmine\world\format\PalettedBlockArray)"), true);

    zend_class_entry *ce;
    if ((ce = zend_lookup_class(className)) != nullptr) {
        object->paletted_block_entry_class = ce;
    } else {
        zend_string_release(className);

        zend_throw_error(nullptr, "ext-chunkutils2 is required for ext-noise to function correctly.");
        RETURN_THROWS();
    }

    new (&object->overworldPopulator) OverworldPopulator();

    zend_string_release(className);

    Biome::initBiomes();
}

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_OverworldChunkPopulator_populateChunk, 0, 5, IS_VOID, 0)
    ZEND_ARG_TYPE_INFO(1, palettedArray, IS_ARRAY, 0)
    ZEND_ARG_TYPE_INFO(0, biomeArray, IS_ARRAY, 0)
    ZEND_ARG_TYPE_INFO(1, dirtyFlags, IS_ARRAY, 0)
    ZEND_ARG_TYPE_INFO(0, morton, IS_LONG, 0)
    ZEND_ARG_OBJ_INFO(0, seed, Random, 0)
ZEND_END_ARG_INFO()

// Overhead cost: 8700ns (If PalettedBlockArray present), 84900ns (If all array values are null)
PHP_METHOD (OverworldChunkPopulator, populateChunk) {
    zval *palettedArray;
    zval *biomeArray;
    zval *dirtyFlags;
    zval *random;
    zend_long morton;
    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 5, 5)
        Z_PARAM_ARRAY_EX(palettedArray, 1, 1)
        Z_PARAM_ARRAY_EX(biomeArray, 1, 1)
        Z_PARAM_ARRAY_EX(dirtyFlags, 1, 1)
        Z_PARAM_LONG(morton)
        Z_PARAM_OBJECT_OF_CLASS(random, random_entry)
    ZEND_PARSE_PARAMETERS_END();

    auto chunkManager = SimpleChunkManager(Y_MIN, Y_MAX);
    auto storage = fetch_from_zend_object<overworld_populators>(Z_OBJ_P(getThis()));

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
            return;
        } else if (Z_TYPE_P(parent_element) != IS_ARRAY) {
            zend_type_error("The array value of index %lld must be an array.", parent_hash);
            return;
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
                    return;
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
                } catch(std::out_of_range const& exc){
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

            auto chunk = new Chunk(static_cast<int64_t>(parent_hash), blockContainers, new BiomeArray(span));
            chunk->setDirty(Z_TYPE_P(hash_index) == IS_TRUE);

            chunkManager.setChunk(chunk->getX(), chunk->getZ(), chunk);
        }
    } ZEND_HASH_FOREACH_END();

    auto randomObject = fetch_from_zend_object<random_obj>(Z_OBJ_P(random));
    auto populator = storage->overworldPopulator;

    try {
        populator.populate(chunkManager, randomObject->random, static_cast<int>(chunkX), static_cast<int>(chunkZ));
    } catch (std::exception &error) {
        chunkManager.destroyObjects();

        zend_throw_error(zend_ce_exception, "**INTERNAL GENERATOR ERROR** %s", error.what());
        RETURN_THROWS();
    }

    zval boolObject;
    for (auto x : chunkManager.getChunks()){
        ZVAL_BOOL(&boolObject, x.second->isDirty());

        zend_hash_index_update(Z_ARRVAL_P(dirtyFlags), static_cast<zend_ulong>(x.first), &boolObject);
    }

    chunkManager.destroyObjects();
}

zend_function_entry overworld_methods[] = {
    PHP_ME(OverworldChunkPopulator, __construct, arginfo_OverworldChunkPopulator___construct, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
    PHP_ME(OverworldChunkPopulator, populateChunk, arginfo_OverworldChunkPopulator_populateChunk, ZEND_ACC_PUBLIC)
    PHP_FE_END
};

void register_overworld_populators() {
    memcpy(&overworld_populator_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
    overworld_populator_handlers.offset = XtOffsetOf(overworld_populators, std);
    overworld_populator_handlers.free_obj = populator_free;

    zend_class_entry cle;
    INIT_CLASS_ENTRY(cle, "OverworldChunkPopulator", overworld_methods);
    cle.create_object = populator_new;
    zend_register_internal_class(&cle);
}