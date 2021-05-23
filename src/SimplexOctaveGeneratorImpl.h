#ifndef SIMPLEX_OCTAVE_GENERATOR_IMPL_H
#define SIMPLEX_OCTAVE_GENERATOR_IMPL_H


extern "C" {
#include "php.h"
#include "ext/spl/spl_exceptions.h"
#include "ext/standard/php_var.h"
#include "Zend/zend_exceptions.h"
}

#include "lib/ZendUtil.h"
#include "lib/octaves/SimplexOctaveGenerator.h"

void register_simplex_octave();

#endif
