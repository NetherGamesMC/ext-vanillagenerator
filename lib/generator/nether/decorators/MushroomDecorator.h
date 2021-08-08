#ifndef EXT_NOISELIB_LIB_GENERATOR_NETHER_DECORATORS_MUSHROOMDECORATOR_H_
#define EXT_NOISELIB_LIB_GENERATOR_NETHER_DECORATORS_MUSHROOMDECORATOR_H_

#include <lib/generator/decorators/Decorator.h>

class MushroomDecorator : public Decorator{
 public:
  MushroomDecorator(MinecraftBlock type) : type(type){}

  void Decorate(ChunkManager &world, Random &random, int_fast32_t chunkX, int_fast32_t chunkZ) override;

 private:
  MinecraftBlock type;
  uint_fast16_t materials[4] = {NETHERRACK.GetId(), NETHER_QUARTZ_ORE.GetId(), SOUL_SAND.GetId(), GRAVEL.GetId()};
};

#endif //EXT_NOISELIB_LIB_GENERATOR_NETHER_DECORATORS_MUSHROOMDECORATOR_H_