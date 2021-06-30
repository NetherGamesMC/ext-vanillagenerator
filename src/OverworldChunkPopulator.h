#ifndef EXT_NOISE_OVERWORLD_CHUNK_POPULATOR_H
#define EXT_NOISE_OVERWORLD_CHUNK_POPULATOR_H

#include <lib/generator/OverworldPopulators.h>

typedef struct {
  OverworldPopulator overworldPopulator;
  zend_class_entry *paletted_block_entry_class;

  zend_object std;
} overworld_populators;

PHP_METHOD (OverworldChunkPopulator, init);
PHP_METHOD (OverworldChunkPopulator, populateChunk);

void register_overworld_populators();

#endif
