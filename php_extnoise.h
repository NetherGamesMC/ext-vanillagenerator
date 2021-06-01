#ifndef PHP_NOISELIB_NOISE_H
#define PHP_NOISELIB_NOISE_H

#include "php.h"

#ifdef ZTS
#include "TSRM.h"
#endif

extern zend_module_entry extnoise_module_entry;
#define phpext_extnoise_ptr &extnoise_module_entry
#define PHP_EXTNOISE_VERSION "1.3.0"

#endif

