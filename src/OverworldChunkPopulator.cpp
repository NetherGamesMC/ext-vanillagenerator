#include <lib/ZendUtil.h>
#include <lib/chunk/Chunk.h>
#include <PhpPalettedBlockArrayObj.h>
#include <lib/pocketmine/Constants.h>
#include <chrono>
#include <iostream>

#include "RandomImpl.h"

#include "lib/generator/NormalPopulators.h"

extern "C" {
    #include <php.h>
    #include <zend_exceptions.h>
    #include <ext/spl/spl_exceptions.h>
}

zend_class_entry *paletted_block_entry_class = nullptr;

ZEND_BEGIN_ARG_INFO_EX(arginfo_OverworldChunkPopulator_init, 0, 0, 0)
ZEND_END_ARG_INFO()

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// For some reason, in other threads of pthreads, the 'paletted_block_entry_class' is defined. I assume that all
/// threads should have their own memory instance and are not shared? Should we be concern about accessing pointers
/// that is not thread safe? Or is it even not thread safe? This is not what I was expecting. This is being tested in
/// Windows, I am not entirely sure about Linux.

PHP_METHOD (OverworldChunkPopulator, init) {
    // Attempt to initialize PalettedBlockArray class entry, if it does not exists,
    // it simply means that the server has no ext-chunkutils2 installed.
    if (paletted_block_entry_class == nullptr) {
        zend_string *className = zend_string_init(ZEND_STRL(R"(\pocketmine\world\format\PalettedBlockArray)"), true);

        zend_class_entry * ce;
        if ((ce = zend_lookup_class(className)) != nullptr) {
            paletted_block_entry_class = ce;
        } else {
            zend_string_release(className);

            zend_throw_error(nullptr, "ext-chunkutils2 is required for ext-noise to function correctly.");
            return;
        }

        zend_string_release(className);

        init_biomes();
        init_populators();
    } else {
        // TODO: Should we really throw an exception here?
    }
}

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_OverworldChunkPopulator_populateChunk, 0, 4, IS_VOID, 0)
    ZEND_ARG_TYPE_INFO(1, palettedArray, IS_ARRAY, 0)
    ZEND_ARG_TYPE_INFO(1, biomeArray, IS_ARRAY, 0)
    ZEND_ARG_TYPE_INFO(0, morton, IS_LONG, 0)
    ZEND_ARG_OBJ_INFO(0, seed, Random, 0)
ZEND_END_ARG_INFO()

// Overhead cost: 8700ns (If PalettedBlockArray present), 84900ns (If all array values are null)
PHP_METHOD (OverworldChunkPopulator, populateChunk) {
    if (paletted_block_entry_class == nullptr) {
        zend_throw_error(nullptr, "populateChunk() was called without being initialized first!");

        return;
    }

    zval *palettedArray;
    zval *biomeArray;
    zval *random;
    zend_long morton;
    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 4, 4)
        Z_PARAM_ARRAY_EX(palettedArray, 1, 1)
        Z_PARAM_ARRAY_EX(biomeArray, 1, 1)
        Z_PARAM_LONG(morton)
        Z_PARAM_OBJECT_OF_CLASS(random, random_entry)
    ZEND_PARSE_PARAMETERS_END();

    auto chunkManager = SimpleChunkManager(Y_MIN, Y_MAX);

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
                if ((!isNull && !isObject) || (isObject && !instanceof_function(Z_OBJCE_P(element), paletted_block_entry_class))) {
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
                        object_init_ex(&new_class, paletted_block_entry_class);

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

            if (!zend_hash_index_exists(Z_ARRVAL_P(biomeArray), parent_hash)){
                zend_throw_error(nullptr, "Chunk for hash %lld does not present in biome array.", parent_hash);
                return;
            }

            zval* biome_array = zend_hash_index_find(Z_ARRVAL_P(biomeArray), parent_hash);

            if(Z_TYPE_P(biome_array) != IS_STRING){
                zend_throw_exception_ex(spl_ce_InvalidArgumentException, 0, "Biome array must be a string, %s given", zend_zval_type_name(biome_array));
                break;
            } else if (Z_STRLEN_P(biome_array) != BiomeArray::DATA_SIZE) {
                zend_throw_exception_ex(spl_ce_InvalidArgumentException, 0, "Biome array is expected to be exactly %zu bytes, but got %zu bytes", BiomeArray::DATA_SIZE, Z_STRLEN_P(biome_array));
                break;
            }

            gsl::span<const uint8_t, BiomeArray::DATA_SIZE> span(reinterpret_cast<const uint8_t *>(Z_STR_P(biome_array)), BiomeArray::DATA_SIZE);

            auto chunk = new Chunk(static_cast<int64_t>(parent_hash), blockContainers, BiomeArray(span));

            chunkManager.setChunk(chunk->getX(), chunk->getZ(), chunk);
        }
    } ZEND_HASH_FOREACH_END();

    auto randomObject = fetch_from_zend_object<random_obj>(Z_OBJ_P(random));

    try {
        auto populator = BiomePopulator();
        populator.initPopulators();

        populator.populate(chunkManager, randomObject->random, static_cast<int>(chunkX), static_cast<int>(chunkZ));
    } catch (std::exception &error) {
        zend_throw_error(zend_ce_exception, "**INTERNAL GENERATOR ERROR** %s", error.what());
    }

    chunkManager.clean();
}

zend_function_entry overworld_methods[] = {
    PHP_ME(OverworldChunkPopulator, init, arginfo_OverworldChunkPopulator_init, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_ME(OverworldChunkPopulator, populateChunk, arginfo_OverworldChunkPopulator_populateChunk, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_FE_END
};

void register_overworld_populators() {
    zend_class_entry cle;
    INIT_CLASS_ENTRY(cle, "OverworldChunkPopulator", overworld_methods);
    zend_register_internal_class(&cle);
}