#ifndef EXT_VANILLAGENERATOR_GLOWSTONEDECORATOR_H
#define EXT_VANILLAGENERATOR_GLOWSTONEDECORATOR_H

#include <lib/generator/overworld/decorators/Decorator.h>

class GlowstoneDecorator : public Decorator {
 public:
  GlowstoneDecorator(bool variableAmount) : variableAmount(variableAmount) {}

  void Decorate(ChunkManager &world, Random &random, int_fast32_t chunkX, int_fast32_t chunkZ) override;

 private:
  bool variableAmount = false;
};
#endif //EXT_VANILLAGENERATOR_GLOWSTONEDECORATOR_H
