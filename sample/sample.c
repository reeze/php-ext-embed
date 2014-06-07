/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2012 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author: Reeze Xia <reeze@php.net>                                    |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php_ext_embed.h"
#include "php_ext_embed_libs.h"

#include <php.h>
#include <ext/standard/info.h>

#include "php_sample.h"


#ifndef PHP_FE_END
# define PHP_FE_END { NULL, NULL, NULL, 0, 0 }
#endif

/* {{{ sample_functions[]
 */
const zend_function_entry sample_functions[] = {
	PHP_FE_END	/* Must be the last line in sample_functions[] */
};
/* }}} */

/* {{{ sample_module_entry
 */
zend_module_entry sample_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"sample",
	sample_functions,
	PHP_MINIT(sample),
	PHP_MSHUTDOWN(sample),
	PHP_RINIT(sample),
	PHP_RSHUTDOWN(sample),
	PHP_MINFO(sample),
#if ZEND_MODULE_API_NO >= 20010901
	PHP_SAMPLE_VERSION,
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_SAMPLE
ZEND_GET_MODULE(sample)
#endif


PHP_RINIT_FUNCTION(sample)
{
	PHP_EXT_EMBED_RINIT(sample);
	return SUCCESS;
}

PHP_RSHUTDOWN_FUNCTION(sample)
{
	PHP_EXT_EMBED_RSHUTDOWN(sample);
	return SUCCESS;
}

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(sample)
{
	PHP_EXT_EMBED_MINIT(sample);
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(sample)
{
	PHP_EXT_EMBED_MSHUTDOWN(sample);
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(sample)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "sample support", "enabled");
	php_info_print_table_row(2, "sample extension version", PHP_SAMPLE_VERSION);
	php_info_print_table_row(2, "sample with embeded library", "yes");
	php_info_print_table_end();
}
/* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
