#ifndef HAVE_CONFIG_H
#include "config.h"
#endif

#include "Noise.h"

extern "C" {
     #include "php.h"
     #include "php_noiselib.h"
}

PHP_MINIT_FUNCTION(noisegen) {
     register_noise_class();
     return SUCCESS;
}

zend_module_entry noisegen_module_entry = {
     STANDARD_MODULE_HEADER,
     "noisegen",
     NULL,
     PHP_MINIT(noisegen),
     NULL,
     NULL,
     NULL,
     NULL,
     PHP_NOISELIB_VERSION,
     STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_NOISELIB
extern "C" {
     ZEND_GET_MODULE(noisegen)
}
#endif