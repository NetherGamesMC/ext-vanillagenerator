dnl config.m4 for extension extnoise

PHP_ARG_WITH(extnoise, whether to enable extnoise support,
[ --enable-extnoise Enable extnoise])

if test "$PHP_EXTNOISE" != "no"; then
  dnl probably won't use this, but will keep just in case
  PHP_REQUIRE_CXX()

  dnl idk 
  PHP_NEW_EXTENSION(extnoise, extnoise.cpp src/PerlinOctaveGeneratorImpl.cpp src/RandomImpl.cpp src/SimplexOctaveGeneratorImpl.cpp src/NoiseMapGenerator.cpp lib/vanilla/VanillaNoise.cpp, $ext_shared,, -DZEND_ENABLE_STATIC_TSRMLS_CACHE=1 -std=c++14 -fno-strict-aliasing -DGSL_THROW_ON_CONTRACT_VIOLATION=1, yes)
  PHP_SUBST(NOISELIB_SHARED_LIBADD)
  PHP_ADD_BUILD_DIR($ext_builddir/src, 1)
  PHP_ADD_BUILD_DIR($ext_builddir/lib, 1)
fi