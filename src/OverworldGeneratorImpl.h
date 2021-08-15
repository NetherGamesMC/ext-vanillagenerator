#ifndef EXT_VANILLAGENERATOR_OVERWORLD_CHUNK_POPULATOR_H
#define EXT_VANILLAGENERATOR_OVERWORLD_CHUNK_POPULATOR_H

#include <lib/generator/overworld/OverworldGenerator.h>

typedef struct {
  OverworldGenerator *overworldGenerator;
  zend_class_entry *paletted_block_entry_class;

  zend_object std;
} overworld_generator;

PHP_METHOD (OverworldGenerator, __construct);
PHP_METHOD (OverworldGenerator, generateChunk);
PHP_METHOD (OverworldGenerator, populateChunk);

void register_overworld_generator();

#endif
