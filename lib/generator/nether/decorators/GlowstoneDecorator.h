#ifndef EXT_NOISELIB_GLOWSTONEDECORATOR_H
#define EXT_NOISELIB_GLOWSTONEDECORATOR_H

#include <lib/generator/decorators/Decorator.h>

class GlowstoneDecorator : public Decorator {
 public:
  GlowstoneDecorator(bool variableAmount) : variableAmount(variableAmount) {}

  void Decorate(ChunkManager &world, Random &random, int_fast32_t chunkX, int_fast32_t chunkZ) override;

 private:
  bool variableAmount = false;
};
#endif //EXT_NOISELIB_GLOWSTONEDECORATOR_H
