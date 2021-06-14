#ifndef EXT_NOISE_OVERWORLD_CHUNK_POPULATOR_H
#define EXT_NOISE_OVERWORLD_CHUNK_POPULATOR_H

extern "C" {
#include <php.h>
#include <zend_exceptions.h>
#include <ext/spl/spl_exceptions.h>
}

#include "lib/generator/NormalPopulators.h"

PHP_METHOD(OverworldChunkPopulator, init);
PHP_METHOD(OverworldChunkPopulator, populateChunk);

void register_overworld_populators();

#endif
