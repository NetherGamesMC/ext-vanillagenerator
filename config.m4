dnl config.m4 for extension extnoise

PHP_ARG_WITH(extnoise, whether to enable extnoise support,
[ --enable-extnoise Enable extnoise])

if test "$PHP_EXTNOISE" != "no"; then
  dnl probably won't use this, but will keep just in case
  PHP_REQUIRE_CXX()

  dnl idk 
  PHP_NEW_EXTENSION(extnoise, extnoise.cpp lib/chunk/Chunk.cpp lib/chunk/SimpleChunkManager.cpp lib/debug/Debug.cpp lib/generator/biomegrid/BiomeEdgeMapLayer.cpp lib/generator/biomegrid/BiomeMapLayer.cpp lib/generator/biomegrid/BiomeThinEdgeMapLayer.cpp lib/generator/biomegrid/BiomeVariationMapLayer.cpp lib/generator/biomegrid/DeepOceanMapLayer.cpp lib/generator/biomegrid/ErosionMapLayer.cpp lib/generator/biomegrid/MapLayer.cpp lib/generator/biomegrid/NoiseMapLayer.cpp lib/generator/biomegrid/RarePlainsMapLayer.cpp lib/generator/biomegrid/RiverMapLayer.cpp lib/generator/biomegrid/ShoreMapLayer.cpp lib/generator/biomegrid/SmoothMapLayer.cpp lib/generator/biomegrid/WhittakerMapLayer.cpp lib/generator/biomegrid/ZoomMapLayer.cpp lib/generator/decorators/DeadBushDecorator.cpp lib/generator/decorators/Decorator.cpp lib/generator/decorators/DoublePlantDecorator.cpp lib/generator/decorators/FlowerDecorator.cpp lib/generator/decorators/LakeDecorator.cpp lib/generator/decorators/MushroomDecorator.cpp lib/generator/decorators/TallGrassDecorator.cpp lib/generator/decorators/TreeDecorator.cpp lib/generator/ground/DirtAndStonePatchGroundGenerator.cpp lib/generator/ground/DirtPatchGroundGenerator.cpp lib/generator/ground/GravelPatchGroundGenerator.cpp lib/generator/ground/GroundGenerator.cpp lib/generator/ground/MesaGroundGenerator.cpp lib/generator/ground/StonePatchGroundGenerator.cpp lib/generator/objects/DoubleTallPlant.cpp lib/generator/objects/Flower.cpp lib/generator/objects/GenericTree.cpp lib/generator/objects/Lake.cpp lib/generator/objects/OreVein.cpp lib/generator/objects/TallGrass.cpp lib/generator/objects/TerrainObjects.cpp lib/generator/OverworldGenerator.cpp lib/generator/OverworldPopulators.cpp lib/generator/populators/BiomePopulator.cpp lib/generator/populators/OrePopulator.cpp lib/objects/Biome.cpp lib/objects/BiomeArray.cpp lib/objects/BiomeHeightManager.cpp lib/objects/MinecraftBlock.cpp lib/objects/VanillaBiomeGrid.cpp lib/random/Random.cpp lib/vanilla/BukkitSimplexNoiseGenerator.cpp lib/vanilla/octaves/BukkitSimplexOctaveGenerator.cpp lib/vanilla/octaves/PerlinOctaveGenerator.cpp lib/vanilla/octaves/ScalableOctaves.cpp lib/vanilla/octaves/SimplexOctaveGenerator.cpp lib/vanilla/PerlinNoise.cpp lib/vanilla/SimplexNoise.cpp lib/vanilla/Utils.cpp lib/vanilla/VanillaNoise.cpp src/Noise.cpp src/OverworldGeneratorImpl.cpp src/RandomImpl.cpp, $ext_shared,, -DZEND_ENABLE_STATIC_TSRMLS_CACHE=1 -std=c++14 -fno-strict-aliasing -DGSL_THROW_ON_CONTRACT_VIOLATION=1, yes)
  PHP_SUBST(NOISELIB_SHARED_LIBADD)
  PHP_ADD_BUILD_DIR($ext_builddir/src, 1)
  PHP_ADD_BUILD_DIR($ext_builddir/lib, 1)
  PHP_ADD_INCLUDE($EXTENSION_DIR/chunkutils2/gsl/include)
  PHP_ADD_INCLUDE($EXTENSION_DIR/chunkutils2)
  PHP_ADD_INCLUDE($EXTENSION_DIR/chunkutils2/src)
fi