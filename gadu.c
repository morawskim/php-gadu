/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2014 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_gadu.h"
#include "libgadu.h"

/* If you declare any globals in php_gadu.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(gadu)
*/

/* True global resources - no need for thread safety here */
static int le_gadu;

/* {{{ gadu_functions[]
 *
 * Every user visible function must have an entry in gadu_functions[].
 */
const zend_function_entry gadu_functions[] = {
        PHP_FE(confirm_gadu_compiled,	NULL)		/* For testing, remove later. */
        PHP_FE(gadu_login,	NULL)		/* For testing, remove later. */
        PHP_FE(gadu_notify,	NULL)		/* For testing, remove later. */
        PHP_FE(gadu_send_message,	NULL)		/* For testing, remove later. */
        PHP_FE(gadu_logoff,	NULL)		/* For testing, remove later. */
        PHP_FE_END	/* Must be the last line in gadu_functions[] */
};
/* }}} */

/* {{{ gadu_module_entry
 */
zend_module_entry gadu_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
        "gadu",
        gadu_functions,
        PHP_MINIT(gadu),
        PHP_MSHUTDOWN(gadu),
        PHP_RINIT(gadu),		/* Replace with NULL if there's nothing to do at request start */
        PHP_RSHUTDOWN(gadu),	/* Replace with NULL if there's nothing to do at request end */
        PHP_MINFO(gadu),
#if ZEND_MODULE_API_NO >= 20010901
	PHP_GADU_VERSION,
#endif
        STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_GADU
ZEND_GET_MODULE(gadu)
#endif

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("gadu.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_gadu_globals, gadu_globals)
    STD_PHP_INI_ENTRY("gadu.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_gadu_globals, gadu_globals)
PHP_INI_END()
*/
/* }}} */

/* {{{ php_gadu_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_gadu_init_globals(zend_gadu_globals *gadu_globals)
{
	gadu_globals->global_value = 0;
	gadu_globals->global_string = NULL;
}
*/
/* }}} */


static void php_gg_dtor(zend_rsrc_list_entry *rsrc TSRMLS_DC) {
    struct gg_session *session = (struct gg_session*)rsrc->ptr;
        gg_logoff(session);
        gg_free_session(session);
}

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(gadu)
{
    /* If you have INI entries, uncomment these lines
    REGISTER_INI_ENTRIES();
    */
    le_gadu = zend_register_list_destructors_ex(
            php_gg_dtor,
            NULL,
            PHP_GG_RES_NAME,
            module_number
    );
    return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(gadu)
{
    /* uncomment this line if you have INI entries
    UNREGISTER_INI_ENTRIES();
    */
    return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(gadu)
{
    return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(gadu)
{
    return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(gadu)
{
    php_info_print_table_start();
    php_info_print_table_header(2, "gadu support", "enabled");
    php_info_print_table_end();

    /* Remove comments if you have entries in php.ini
    DISPLAY_INI_ENTRIES();
    */
}
/* }}} */


/* Remove the following function when you have successfully modified config.m4
   so that your module can be compiled into PHP, it exists only for testing
   purposes. */

/* Every user-visible function in PHP should document itself in the source */
/* {{{ proto string confirm_gadu_compiled(string arg)
   Return a string to confirm that the module is compiled in */
PHP_FUNCTION(confirm_gadu_compiled)
{
    char *arg = NULL;
    int arg_len, len;
    char *strg;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &arg, &arg_len) == FAILURE) {
        return;
    }

    len = spprintf(&strg, 0, "Congratulations! You have successfully modified ext/%.78s/config.m4. Module %.78s is now compiled into PHP.", "gadu", arg);
    RETURN_STRINGL(strg, len, 0);
}
/* }}} */
/* The previous line is meant for vim and emacs, so it can correctly fold and
   unfold functions in source code. See the corresponding marks just before
   function definition, where the functions purpose is also documented. Please
   follow this convention for the convenience of others editing your code.
*/

PHP_FUNCTION(gadu_login)
{
    struct gg_login_params loginParams;
    memset(&loginParams, 0, sizeof(loginParams));

    char* ptrPassword = 0;
    int passwordLength = 0;
    uint32_t uin = 0;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ls", &uin, &ptrPassword, &passwordLength) == FAILURE) {
        return;
    }

    loginParams.async = 0;
    loginParams.status = GG_STATUS_INVISIBLE;
    loginParams.uin = uin;
    loginParams.password = ptrPassword;

    struct gg_session *session = gg_login(&loginParams);
    if (session) {
        ZEND_REGISTER_RESOURCE(return_value, session, le_gadu);
    } else {
        RETURN_FALSE;
    }
}

PHP_FUNCTION(gadu_notify)
{
    struct gg_session *session;
    zval *gg_resource;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &gg_resource) == FAILURE) {
        RETURN_FALSE;
    }

    ZEND_FETCH_RESOURCE(session, struct gg_session*, &gg_resource, -1, PHP_GG_RES_NAME, le_gadu);
    int res = gg_notify(session, NULL, 0);
    if (res) {
        RETURN_FALSE;
    }

    RETURN_TRUE;
}


PHP_FUNCTION(gadu_send_message)
{
    struct gg_session *session;
    zval *gg_resource;

    char *ptr_message = 0;
    int message_length = 0;
    uint32_t uin = 0;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rls", &gg_resource, &uin, &ptr_message, &message_length) == FAILURE) {
        RETURN_FALSE;
    }

    ZEND_FETCH_RESOURCE(session, struct gg_session*, &gg_resource, -1, PHP_GG_RES_NAME, le_gadu);

    if (message_length > 0) {
        int res = gg_send_message(session, GG_CLASS_MSG, uin, (unsigned char*)ptr_message);
        if (res) {
            RETURN_FALSE;
        }
        RETURN_TRUE;
    } else {
        RETURN_FALSE
    }
}

PHP_FUNCTION(gadu_logoff)
{
    struct gg_session *session;
    zval *gg_resource;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &gg_resource) == FAILURE) {
        RETURN_FALSE;
    }

    ZEND_FETCH_RESOURCE(session, struct gg_session*, &gg_resource, -1, PHP_GG_RES_NAME, le_gadu);
    zend_hash_index_del(&EG(regular_list), Z_RESVAL_P(gg_resource));

    RETURN_TRUE;
}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
