#ifndef PHP_NOISELIB_NOISE_H
#define PHP_NOISELIB_NOISE_H

#include "php.h"

#ifdef ZTS
#include "TSRM.h"
#endif

extern zend_module_entry mcgenerator_module_entry;
#define phpext_mcgenerator_ptr &mcgenerator_module_entry
#define PHP_MCGENERATOR_VERSION "2.1.0"

#endif

