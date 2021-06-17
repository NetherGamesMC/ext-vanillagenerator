#include <lib/ZendUtil.h>
#include <lib/chunk/Chunk.h>
#include <PhpPalettedBlockArrayObj.h>
#include <lib/pocketmine/Constants.h>
#include <chrono>

#include "OverworldChunkPopulator.h"

zend_class_entry *paletted_block_entry_class = nullptr;

ZEND_BEGIN_ARG_INFO_EX(arginfo_OverworldChunkPopulator_init, 0, 0, 0)
ZEND_END_ARG_INFO()

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

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_OverworldChunkPopulator_populateChunk, 0, 3, IS_VOID, 0)
    ZEND_ARG_TYPE_INFO(1, array, IS_ARRAY, 0)
    ZEND_ARG_TYPE_INFO(0, morton, IS_LONG, 0)
    ZEND_ARG_TYPE_INFO(0, biome_array, IS_STRING, 0)
ZEND_END_ARG_INFO()

// Overhead cost: 23700ns
PHP_METHOD (OverworldChunkPopulator, populateChunk) {
    if (paletted_block_entry_class == nullptr) {
        zend_throw_error(nullptr, "populateChunk() was called without being initialized first!");

        return;
    }

    zval *array;
    zend_long morton;
    zend_string* biome_array;
    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 3, 3)
        Z_PARAM_ARRAY_EX(array, 1, 1)
        Z_PARAM_LONG(morton)
        Z_PARAM_STR(biome_array)
    ZEND_PARSE_PARAMETERS_END();

    if (ZSTR_LEN(biome_array) != BiomeArray::DATA_SIZE) {
        zend_throw_exception_ex(spl_ce_InvalidArgumentException, 0, "Biome array is expected to be exactly %zu bytes, but got %zu bytes", BiomeArray::DATA_SIZE, ZSTR_LEN(biome_array));
    }

    gsl::span<const uint8_t, BiomeArray::DATA_SIZE> span(reinterpret_cast<const uint8_t *>(ZSTR_VAL(biome_array)), BiomeArray::DATA_SIZE);

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

    // We do not use "new" because the object will remain existence until the program ends,
    // which we do not want it to behave, however it is easier to allocate the object without "new"
    // since these objects will be destroyed after it goes out of scope. Simply say, it will avoid uncertainty memory leaks.
    auto chunkManager = SimpleChunkManager(Y_MIN, Y_MAX);
    auto chunk = Chunk(morton, blockContainers, BiomeArray(span));

    chunkManager.setChunk(chunk.getX(), chunk.getZ(), &chunk);

    try{
        Random random = Random(1234);
        LakeDecorator decorator = LakeDecorator(STILL_WATER, 1);

        // This does run, but I do not sure if it does changes anything to our chunk
        decorator.decorate(chunkManager, random, 0, 0);
    } catch (std::exception &error) {
        zend_throw_error(zend_ce_exception, "**INTERNAL GENERATOR ERROR** %s", error.what());
    }
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