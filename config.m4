dnl config.m4 for extension noiselib

PHP_ARG_WITH(noiselib, whether to enable noiselib support,
[ --enable-noiselib Enable Noiselib])

if test "$PHP_NOISELIB" != "no"; then
  PHP_REQUIRE_CXX()

  dnl idk 
  PHP_NEW_EXTENSION(noiselib, php_noiselib.cpp Noise.cpp bukkit/perlin_noise.cpp, $ext_shared,, -DZEND_ENABLE_STATIC_TSRMLS_CACHE=1 -std=c++14 -fno-strict-aliasing -DGSL_THROW_ON_CONTRACT_VIOLATION=1, yes)
  PHP_SUBST(NOISELIB_SHARED_LIBADD)
  PHP_ADD_BUILD_DIR($ext_builddir/src, 1)
  PHP_ADD_BUILD_DIR($ext_builddir/lib, 1)
  PHP_ADD_INCLUDE($ext_builddir)
  PHP_ADD_INCLUDE($ext_builddir/gsl/include)
fi