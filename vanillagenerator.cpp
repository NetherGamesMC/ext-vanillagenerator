#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

extern "C" {
#include "php.h"
#include "ext/standard/info.h"
#include "php_vanillagenerator.h"
}

#include "src/RandomImpl.h"
#include "src/Noise.h"
#include "src/OverworldGeneratorImpl.h"

PHP_MINIT_FUNCTION (vanillagenerator) {
  register_internal_randoms();
  register_noise_function();
  register_overworld_generator();
  return SUCCESS;
}

PHP_MINFO_FUNCTION (vanillagenerator) {
  php_info_print_table_start();
  php_info_print_table_header(2, "vanillagenerator support", "enabled");
  php_info_print_table_end();
}

zend_module_entry vanillagenerator_module_entry = {
    STANDARD_MODULE_HEADER,
    "vanillagenerator",
    NULL,
    PHP_MINIT(vanillagenerator),
    NULL,
    NULL,
    NULL,
    PHP_MINFO(vanillagenerator),
    PHP_VANILLAGENERATOR_VERSION,
    STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_VANILLAGENERATOR
extern "C" {
#ifdef ZTS
     ZEND_TSRMLS_CACHE_DEFINE()
#endif
     ZEND_GET_MODULE(vanillagenerator)
}
#endif