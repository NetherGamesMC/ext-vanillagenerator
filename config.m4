dnl config.m4 for extension noiselib

PHP_ARG_WITH(noisegen, whether to enable noiselib support, [ --enable-extnoise Enable Noiselib])

if test "$PHP_NOISELIB" != "no"; then
  dnl probably won't use this, but will keep just in case
  PHP_REQUIRE_CXX()

  dnl idk 
  PHP_NEW_EXTENSION(noiselib, noiselib.cpp Noise.cpp, $ext_shared,, -DZEND_ENABLE_STATIC_TSRMLS_CACHE=1 -std=c++14 -fno-strict-aliasing -DGSL_THROW_ON_CONTRACT_VIOLATION=1, yes)
  PHP_SUBST(NOISELIB_SHARED_LIBADD)
  dnl PHP_ADD_BUILD_DIR($ext_builddir/src, 1)
  dnl PHP_ADD_BUILD_DIR($ext_builddir/lib, 1)
  dnl PHP_ADD_INCLUDE($ext_builddir)
  dnl PHP_ADD_INCLUDE($ext_builddir/gsl/include)
fi