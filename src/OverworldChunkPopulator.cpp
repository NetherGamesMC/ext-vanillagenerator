#include <string>
#include <lib/ZendUtil.h>
#include <lib/chunk/Chunk.h>
#include <PhpPalettedBlockArrayObj.h>

#include "OverworldChunkPopulator.h"

zend_class_entry *paletted_block_entry_class = nullptr;

ZEND_BEGIN_ARG_INFO_EX(arginfo_OverworldChunkPopulator_init, 0, 0, 0)
ZEND_END_ARG_INFO()

PHP_METHOD (OverworldChunkPopulator, init) {
    // Attempt to initialize PalettedBlockArray class entry, if it does not exists,
    // it simply means that the server has no ext-chunkutils2 installed.
    if (paletted_block_entry_class == nullptr) {
        std::string classFormattedName = R"(\pocketmine\world\format\PalettedBlockArray)";
        zend_string *className = zend_string_init(classFormattedName.data(), classFormattedName.size(), true);

        zend_class_entry * ce;
        if ((ce = zend_lookup_class(className)) != nullptr) {
            paletted_block_entry_class = ce;
        } else {
            zend_string_release(className);

            zend_throw_error(nullptr, "ext-chunkutils2 is required for ext-noise to function correctly.");
            return;
        }

        zend_string_release(className);

        // TODO: Initialize biomes, classes and all related things here.
    } else {
        // TODO: Should we really throw an exception here?
    }
}

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_OverworldChunkPopulator_populateChunk, 0, 1, IS_VOID, 0)
    ZEND_ARG_TYPE_INFO(1, array, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

PHP_METHOD (OverworldChunkPopulator, populateChunk) {
    if (paletted_block_entry_class == nullptr) {
        zend_throw_error(nullptr, "populateChunk() was called without being initialized first!");

        return;
    }

    zval *array;
    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
        Z_PARAM_ARRAY_EX(array, 1, 1)
    ZEND_PARSE_PARAMETERS_END();

    zend_array *hashTable = Z_ARRVAL_P(array);

    zval new_class;
    zval *element;
    zend_string *key;
    zend_ulong hash;

    std::vector<NormalBlockArrayContainer*> blockContainers;
    ZEND_HASH_FOREACH_KEY_VAL(hashTable, hash, key, element) {
        bool isNull = Z_TYPE_P(element) == IS_NULL;
        bool isObject = Z_TYPE_P(element) == IS_OBJECT;
        if ((!isNull && !isObject) || (isObject && !instanceof_function(Z_OBJCE_P(element), paletted_block_entry_class))) {
            if (key == nullptr) {
                zend_type_error(R"(The array index in key %lld must be of type \pocketmine\world\format\PalettedBlockArray|null, %s given)", hash, zend_zval_type_name(element));
            } else {
                zend_type_error(R"(The array index in key '%s' must be of type \pocketmine\world\format\PalettedBlockArray|null, %s given)", key->val, zend_zval_type_name(element));
            }
            break;
        }

        if (!isNull) {
            auto object = fetch_from_zend_object<paletted_block_array_obj>(Z_OBJ_P(element));
            blockContainers.emplace_back(&object->container);
        } else {
            object_init_ex(&new_class, paletted_block_entry_class);

            auto object = fetch_from_zend_object<paletted_block_array_obj>(Z_OBJ_P(&new_class));
            new (&object->container) NormalBlockArrayContainer((Block)0);

            blockContainers.emplace_back(&object->container);

            if (key) {
                zend_hash_update(hashTable, key, &new_class);
            } else {
                zend_hash_index_update(hashTable, hash, &new_class);
            }
        }
    } ZEND_HASH_FOREACH_END();

    auto chunk = Chunk(blockContainers); // TODO: Do something here with our chunks :)

    // It works!
    chunk.setFullBlock(0, 6, 0, (Block)24);
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