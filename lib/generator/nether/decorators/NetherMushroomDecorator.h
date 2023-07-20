#ifndef EXT_VANILLAGENERATOR_LIB_GENERATOR_NETHER_DECORATORS_NETHERMUSHROOMDECORATOR_H_
#define EXT_VANILLAGENERATOR_LIB_GENERATOR_NETHER_DECORATORS_NETHERMUSHROOMDECORATOR_H_

#include <lib/generator/overworld/decorators/Decorator.h>

class NetherMushroomDecorator : public Decorator {
 public:
  NetherMushroomDecorator(const blocks::MCBlock *type) : type(type) {}

  void Decorate(ChunkManager &world, Random &random, int_fast32_t chunkX, int_fast32_t chunkZ) override;

 private:
  const blocks::MCBlock *type;
};

#endif //EXT_VANILLAGENERATOR_LIB_GENERATOR_NETHER_DECORATORS_NETHERMUSHROOMDECORATOR_H_