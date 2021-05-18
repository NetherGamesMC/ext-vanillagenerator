#ifndef HAVE_CONFIG_H
#include "config.h"
#endif

#include "Noise.h"

extern "C" {
     #include "php.h"
     #include "php_noiselib.h"
}

PHP_MINIT_FUNCTION(noiselib) {
     register_noise_class();
     return SUCCESS;
}

zend_module_entry noiselib_module_entry = {
     STANDARD_MODULE_HEADER,
     "noiselib",
     NULL,
     PHP_MINIT_FUNCTION(noiselib),
     NULL,
     NULL,
     NULL,
     NULL,
     PHP_NOISELIB_VERSION,
     STANDARD_MODULE_PROPERTIES
};

extern "C" {
     ZEND_GET_MODULE(noiselib)
}