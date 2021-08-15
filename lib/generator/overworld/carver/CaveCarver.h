#ifndef EXT_VANILLAGENERATOR_LIB_GENERATOR_OVERWORLD_CARVER_CAVECARVER_H_
#define EXT_VANILLAGENERATOR_LIB_GENERATOR_OVERWORLD_CARVER_CAVECARVER_H_

#include <lib/generator/Populator.h>
#include <lib/objects/math/Vector3.h>

class CaveCarver {
 public:
  void Generate(ChunkManager &manager, Random &random, int_fast32_t chunkX, int_fast32_t chunkZ, Chunk *chunk);

  void recursiveGenerate(int_fast32_t chunkX, int_fast32_t chunkZ, int_fast32_t originalChunkX, int_fast32_t originalChunkZ, Chunk *chunk, bool addRooms);

  void addRoom(int_fast64_t seed, int_fast32_t originChunkX, int_fast32_t originChunkZ, Chunk *chunk, double caveStartX, double caveStartY, double caveStartZ);
  void addTunnel(int_fast64_t seed, int_fast32_t originChunkX, int_fast32_t originChunkZ, Chunk *chunk, double caveStartX, double caveStartY, double caveStartZ, float width, float yaw, float pitch, int_fast32_t startCounter, int_fast32_t endCounter, double heightModifier);

  static void digBlock(Chunk *chunk, int_fast8_t currX, int_fast16_t currY, int_fast8_t currZ);
  static bool canReplaceBlock(Chunk *chunk, int_fast8_t currX, int_fast16_t currY, int_fast8_t currZ);
 private:
  static const int_fast32_t chunkRadius;
  static const int_fast32_t density;
  static const int_fast32_t topY;
  static const int_fast32_t bottomY;
  static const int_fast32_t caveLiquidAltitude;

  Random rand = {0};
};

#endif //EXT_VANILLAGENERATOR_LIB_GENERATOR_OVERWORLD_CARVER_CAVECARVER_H_
