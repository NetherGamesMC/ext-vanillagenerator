#ifndef EXT_VANILLAGENERATOR_NETHERGENERATOR_H
#define EXT_VANILLAGENERATOR_NETHERGENERATOR_H

#include <lib/generator/overworld/OverworldGenerator.h>

struct NetherWorldOctaves {
  PerlinOctaveGenerator height;
  PerlinOctaveGenerator roughness;
  PerlinOctaveGenerator roughness2;
  PerlinOctaveGenerator detail;
  SimplexOctaveGenerator surface;
  PerlinOctaveGenerator soulSand;
  PerlinOctaveGenerator gravel;
};

class NetherGenerator {
 public:
  explicit NetherGenerator(int_fast64_t seed);
 private:
  TerrainDensity GenerateTerrainDensity(int_fast32_t x, int_fast32_t z);

  void GenerateChunkData(ChunkManager &world, int_fast32_t x, int_fast32_t z);
  void GenerateRawTerrain(ChunkManager &world, int_fast32_t x, int_fast32_t z);
  void GenerateTerrainColumn(ChunkManager &world, int_fast32_t x, int_fast32_t z, double surfaceNoise, double soulSandNoise, double gravelNoise);

  static int_fast32_t DensityHash(int_fast32_t i, int_fast32_t j, int_fast32_t k);

  Random random_;
  Random octaveRandom_;
  NetherWorldOctaves octaves_;
};

#endif //EXT_VANILLAGENERATOR_NETHERGENERATOR_H
