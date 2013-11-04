#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <php.h>
#include <php_main.h>
#include <ext/standard/php_string.h>
#include <php_ini.h>
#include <ext/standard/info.h>
#include "wms_helper.h"

static zend_function_entry wms_helper_functions[] = {
    ZEND_FE(coord2pix, NULL)
    ZEND_FE(coords2pix, NULL)
    ZEND_FE(points2pix, NULL)
    //ZEND_FE(hello_foreach, NULL)
    ZEND_FE(hello_array_strings, NULL)
    {NULL, NULL, NULL}
};

zend_module_entry wms_helper_module_entry = {
    STANDARD_MODULE_HEADER,
    PHP_WMS_HELPER_EXTNAME,
    wms_helper_functions,
    PHP_MINIT(wms_helper),
    PHP_MSHUTDOWN(wms_helper),
    NULL,
    NULL,
    PHP_MINFO(wms_helper),
    PHP_WMS_HELPER_VERSION,
    STANDARD_MODULE_PROPERTIES
};


PHP_RINIT_FUNCTION(wms_helper) {
    return SUCCESS;
}

PHP_MINIT_FUNCTION(wms_helper) {
    //REGISTER_INI_ENTRIES();
    return SUCCESS;
}

PHP_MINFO_FUNCTION(wms_helper) {
    php_info_print_table_start();
    php_info_print_table_header(2, "WMS-Helper module", "enabled");
    php_info_print_table_row(2, "WMS-Helper version", "1.0 alpha");
    php_info_print_table_row(2, "Author", "Swen Zanon");
    php_info_print_table_end();
    DISPLAY_INI_ENTRIES();
}

PHP_MSHUTDOWN_FUNCTION(wms_helper) {
    UNREGISTER_INI_ENTRIES();
    return SUCCESS;
}

#ifdef COMPILE_DL_WMS_HELPER
ZEND_GET_MODULE(wms_helper)
#endif


/*static int hello_foreach_static(zval **param, char *greeting TSRMLS_DC) {
    if (Z_TYPE_PP(param) == IS_STRING) {
        php_printf("%s %s\n", greeting, Z_STRVAL_PP(param));
        return ZEND_HASH_APPLY_KEEP;
    } else {
        php_error_docref(NULL TSRMLS_CC, E_WARNING, "Non-string value passed in $names array");
        return ZEND_HASH_APPLY_STOP;
    }
}*/

/*double do_hello_factorial(double val) {
    if (val <= 1) return 1;   
    return val * do_hello_factorial(val - 1);
}*/

/* {{{ proto int hello_factorial(float num)
*   Print the factorial of a given number */
/*ZEND_FUNCTION(hello_factorial)
{
    double val;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "d", &val) == FAILURE) {
        return;
    }

    RETURN_DOUBLE(do_hello_factorial(val));
}*/
/* }}} */


static zval* coord2pix_static(zval *xy_arr_p, double minX, double minY, double resX, double resY )
{
    zval *coord;
    zval **x, **y;
    HashTable *xy_hash = Z_ARRVAL_P(xy_arr_p);
    
    if (zend_hash_index_find(xy_hash, 0, (void **) &x) == SUCCESS &&
        zend_hash_index_find(xy_hash, 1, (void **) &y) == SUCCESS) {

        convert_to_double_ex(x);
        convert_to_double_ex(y);
        
        MAKE_STD_ZVAL(coord);
        array_init(coord);
        add_index_double(coord, 0, (Z_DVAL(**x) - minX) * resX);
        add_index_double(coord, 1, (Z_DVAL(**y) - minY) * resY);
    }
    return coord;
}

/**
 * 
 * @param ht
 * @param return_value
 * @param return_value_ptr
 * @param this_ptr
 * @param return_value_used
 */
ZEND_FUNCTION(coord2pix) {
    
    zval *xy_str_p, *delimiter;
    double minX, minY, resX, resY;
    
    MAKE_STD_ZVAL(xy_str_p);
    
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zdddd", &xy_str_p, &minX, &minY, &resX, &resY) == FAILURE) {
        RETURN_NULL();
    }
    
    ALLOC_INIT_ZVAL(delimiter);
    ZVAL_STRING(delimiter, " ", 1);
    array_init(return_value);
    
    zval *xy_arr_p;
    
    ALLOC_INIT_ZVAL(xy_arr_p);
    array_init(xy_arr_p);

    php_explode(delimiter, xy_str_p, xy_arr_p, 1);
    
    if (Z_TYPE_P(xy_arr_p) == IS_ARRAY) {
        add_next_index_zval(return_value, coord2pix_static(xy_arr_p, minX, minY, resX, resY) );
    }
}

//zend_hash_apply_with_arguments(xy_hash, (apply_func_args_t)php_hello_array_walk_args, 4, minX, minY, resX, resY);
/**
 * 
 * @param string srcDefn
 * @param string tgtDefn
 * @param array points
 */
ZEND_FUNCTION(coords2pix) {
    
    zval *delimiter, *delimiter2, *pts_arr, *str;
    double minX, minY, resX, resY;

    zval **xy_str_pp;
    HashTable *pts_hash;
    //HashPosition pointer;

    zval *xy_str_p, *xy_arr_p;
    
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zdddd", &str, &minX, &minY, &resX, &resY) == FAILURE) {
        RETURN_NULL();
    }
    
    /*
     * ALLOC_ZVAL(zp)Allocate a zval using emalloc()
     * INIT_PZVAL(zp)Set reference count and isref 0
     * INIT_ZVAL(zval)Initialize and set NULL, no pointer
     * ALLOC_INIT_ZVAL(zp)Allocate and initialize a zval
     * MAKE_STD_ZVAL(zp)Allocate, initialize and set NULL
     */
    ALLOC_INIT_ZVAL(delimiter);
    ZVAL_STRING(delimiter, ",", 1);
    
    ALLOC_INIT_ZVAL(delimiter2);
    ZVAL_STRING(delimiter2, " ", 1);

    ALLOC_INIT_ZVAL(pts_arr);
    array_init(pts_arr);
    php_explode(delimiter, str, pts_arr, LONG_MAX);

    //if (Z_TYPE_P(pts_arr) == IS_ARRAY) {
        
        array_init(return_value);
        ALLOC_INIT_ZVAL(xy_arr_p);
        
        pts_hash = Z_ARRVAL_P(pts_arr);
        while (zend_hash_get_current_data(pts_hash, (void **)&xy_str_pp) == SUCCESS) {
        //for (zend_hash_internal_pointer_reset_ex(pts_hash, &pointer);
        //    zend_hash_get_current_data_ex(pts_hash, (void**) &xy_str_pp, &pointer) == SUCCESS;
        //    zend_hash_move_forward_ex(pts_hash, &pointer)) {
            
            //if (Z_TYPE_PP(xy_str_pp) == IS_STRING) {
                
                ALLOC_INIT_ZVAL(xy_str_p);
                MAKE_COPY_ZVAL(xy_str_pp, xy_str_p);
                
                ALLOC_INIT_ZVAL(xy_arr_p);
                array_init(xy_arr_p);
                
                php_explode(delimiter2, xy_str_p, xy_arr_p, LONG_MAX);
                
                if (Z_TYPE_P(xy_arr_p) == IS_ARRAY) {
                    
                    add_next_index_zval(return_value, coord2pix_static(xy_arr_p, minX, minY, resX, resY) );
                }
                zval_ptr_dtor(&xy_str_p);
                zval_ptr_dtor(&xy_arr_p);
            //}
            zend_hash_move_forward(pts_hash);
        }
    //}

    zval_ptr_dtor(&delimiter);
    zval_ptr_dtor(&delimiter2);
}

/**
 * 
 * @param array points
 * @param double minX
 * @param double minY
 * @param double resX
 * @param double resY
 */
ZEND_FUNCTION(points2pix) {
    
    zval *delimiter, *pts_arr;
    double minX, minY, resX, resY;

    zval **xy_str_pp;
    HashTable *pts_hash;
    //HashPosition pointer;

    zval *xy_str_p, *xy_arr_p;
    
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "adddd", &pts_arr, &minX, &minY, &resX, &resY) == FAILURE) {
        RETURN_NULL();
    }
    
    /*
     * ALLOC_ZVAL(zp)Allocate a zval using emalloc()
     * INIT_PZVAL(zp)Set reference count and isref 0
     * INIT_ZVAL(zval)Initialize and set NULL, no pointer
     * ALLOC_INIT_ZVAL(zp)Allocate and initialize a zval
     * MAKE_STD_ZVAL(zp)Allocate, initialize and set NULL
     */
    ALLOC_INIT_ZVAL(delimiter);
    ZVAL_STRING(delimiter, " ", 1);

    if (Z_TYPE_P(pts_arr) == IS_ARRAY) {
        
        array_init(return_value);
        
        pts_hash = Z_ARRVAL_P(pts_arr);
        while (zend_hash_get_current_data(pts_hash, (void **)&xy_str_pp) == SUCCESS) {

            //if (Z_TYPE_PP(xy_str_pp) == IS_STRING) {
                ALLOC_INIT_ZVAL(xy_str_p);
                MAKE_COPY_ZVAL(xy_str_pp, xy_str_p);
                
                ALLOC_INIT_ZVAL(xy_arr_p);
                array_init(xy_arr_p);
                php_explode(delimiter, xy_str_p, xy_arr_p, LONG_MAX);
                
                
                if (Z_TYPE_P(xy_arr_p) == IS_ARRAY) {
                    
                    add_next_index_zval(return_value, coord2pix_static(xy_arr_p, minX, minY, resX, resY) );
                }
                zval_ptr_dtor(&xy_str_p);
                zval_ptr_dtor(&xy_arr_p);
            //}
            zend_hash_move_forward(pts_hash);
        }
    }
    
    zval_ptr_dtor(&delimiter);
}

/*
PHP_FUNCTION(hello_foreach) {
    zval *names;
    char *greet;
    int greet_len;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "as", &names, &greet, &greet_len) == FAILURE) {
        return;
    }
    zend_hash_apply_with_argument(Z_ARRVAL_P(names), (apply_func_arg_t) hello_foreach_static, greet TSRMLS_CC);
}
*/

PHP_FUNCTION(hello_array_strings) {
    zval *arr, **data;
    HashTable *arr_hash;
    HashPosition pointer;
    int array_count;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &arr) == FAILURE) {
        RETURN_NULL();
    }

    arr_hash = Z_ARRVAL_P(arr);
    array_count = zend_hash_num_elements(arr_hash);
    php_printf("The array passed contains %d elements", array_count);

    for (zend_hash_internal_pointer_reset_ex(arr_hash, &pointer);
            zend_hash_get_current_data_ex(arr_hash, (void**) &data, &pointer) == SUCCESS;
            zend_hash_move_forward_ex(arr_hash, &pointer)) {

        if (Z_TYPE_PP(data) == IS_STRING) {
            PHPWRITE(Z_STRVAL_PP(data), Z_STRLEN_PP(data));
            php_printf("<br>\n");
        }
    }
    RETURN_TRUE;
}
