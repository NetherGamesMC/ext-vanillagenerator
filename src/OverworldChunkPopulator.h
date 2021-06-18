#ifndef EXT_NOISE_OVERWORLD_CHUNK_POPULATOR_H
#define EXT_NOISE_OVERWORLD_CHUNK_POPULATOR_H

PHP_METHOD(OverworldChunkPopulator, init);
PHP_METHOD(OverworldChunkPopulator, populateChunk);

void register_overworld_populators();

#endif
