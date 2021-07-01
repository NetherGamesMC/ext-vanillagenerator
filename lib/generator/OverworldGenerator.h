#ifndef EXT_NOISELIB_LIB_GENERATOR_OVERWORLDGENERATOR_H_
#define EXT_NOISELIB_LIB_GENERATOR_OVERWORLDGENERATOR_H_

#include <lib/vanilla/octaves/PerlinOctaveGenerator.h>
#include <lib/vanilla/octaves/SimplexOctaveGenerator.h>
#include <lib/chunk/SimpleChunkManager.h>

typedef std::vector<double> TerrainDensity;

struct WorldOctaves {
  PerlinOctaveGenerator height;
  PerlinOctaveGenerator roughness;
  PerlinOctaveGenerator roughness_2;
  PerlinOctaveGenerator detail;
  SimplexOctaveGenerator surface;
};

class OverworldGenerator {
 public:
  /**
   *  @brief The constructor for Overworld generator.
   *  @param seed     A pseudo-random number generator seed
   */
  explicit OverworldGenerator(int_fast64_t seed);

  /**
   *  @brief Terrain generation of 1 chunk area
   *  @param world    The world chunk manager class
   *  @param chunk_x  The chunk x coordinates
   *  @param chunk_z  The chunk z coordinates
   *
   *  This will generate a terrain of 16x16 blocks wide, this function should only
   *  generate its terrain within chunk coordinates radius.
   */
  void GenerateChunk(SimpleChunkManager &world, int chunk_x, int chunk_z);

  /**
   *  @brief Terrain population of 3x3 chunks wide area
   *  @param world    The world chunk manager class
   *  @param chunk_x  The chunk x coordinates
   *  @param chunk_z  The chunk z coordinates
   *
   *  This will try to populate chunk for the given coordinates. It can however
   *  generate blocks outside the given chunk coordinates bound but no further than
   *  1 neighbouring chunk.
   */
  void PopulateChunk(SimpleChunkManager &world, int chunk_x, int chunk_z);
 private:
  TerrainDensity GenerateTerrainDensity(int x, int z);

  void GenerateChunkData(SimpleChunkManager &world, int chunk_x, int chunk_z, const VanillaBiomeGrid &biome);
  void GenerateRawTerrain(SimpleChunkManager &world, int chunk_x, int chunk_z);

  static int DensityHash(int i, int j, int k);
  static int ElevationWeightHash(int x, int z);

  GridBiome::MapLayerPair map_layer_;

  Random random_;        // Will be used for terrain population.
  Random octave_random_; // This is used for octaves generation, used internally
  WorldOctaves *octaves_;

  std::map<int, float> elevation_weight_;
};

#endif //EXT_NOISELIB_LIB_GENERATOR_OVERWORLDGENERATOR_H_
