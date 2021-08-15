#ifndef EXT_VANILLAGENERATOR_SRC_NETHERGENERATORIMPL_H_
#define EXT_VANILLAGENERATOR_SRC_NETHERGENERATORIMPL_H_

#include <lib/generator/nether/NetherGenerator.h>

typedef struct {
  NetherGenerator *overworldGenerator;
  zend_class_entry *paletted_block_entry_class;

  zend_object std;
} nether_generator;

PHP_METHOD (NetherGenerator, __construct);
PHP_METHOD (NetherGenerator, generateChunk);
PHP_METHOD (NetherGenerator, populateChunk);

void register_nether_generator();

#endif //EXT_VANILLAGENERATOR_SRC_NETHERGENERATORIMPL_H_
