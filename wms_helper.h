#ifndef PHP_WMS_HELPER_H
#define PHP_WMS_HELPER_H 1

#define PHP_WMS_HELPER_VERSION "1.0"
#define PHP_WMS_HELPER_EXTNAME "WMS-Helper"

#define PHP_WMS_HELPER_RES_NAME "WMS-Helper"

#ifdef ZTS
#include <php5/TSRM/TSRM.h>
#endif


PHP_MINIT_FUNCTION(wms_helper);
PHP_MSHUTDOWN_FUNCTION(wms_helper);
PHP_RINIT_FUNCTION(wms_helper);
PHP_MINFO_FUNCTION(wms_helper);

ZEND_FUNCTION(coord2pix);
ZEND_FUNCTION(coords2pix);
ZEND_FUNCTION(points2pix);
//ZEND_FUNCTION(hello_foreach);
ZEND_FUNCTION(hello_array_strings);

extern zend_module_entry wms_helper_module_entry;
#define phpext_proj4_ptr &wms_helper_module_entry

#endif
