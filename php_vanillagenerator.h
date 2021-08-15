#ifndef PHP_VANILLAGENERATOR_H
#define PHP_VANILLAGENERATOR_H

#include "php.h"

#ifdef ZTS
#include "TSRM.h"
#endif

extern zend_module_entry vanillagenerator_module_entry;
#define phpext_vanillagenerator_ptr &vanillagenerator_module_entry
#define PHP_VANILLAGENERATOR_VERSION "2.0.0"

#endif

