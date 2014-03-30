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

#ifndef PHP_SAMPLE_H
#define PHP_SAMPLE_H

#define PHP_SAMPLE_VERSION "0.0.1"

extern zend_module_entry sample_module_entry;
#define phpext_sample_ptr &sample_module_entry

#ifdef ZTS
#include "TSRM.h"
#endif

PHP_MINIT_FUNCTION(sample);
PHP_MSHUTDOWN_FUNCTION(sample);
PHP_RINIT_FUNCTION(sample);
PHP_RSHUTDOWN_FUNCTION(sample);
PHP_MINFO_FUNCTION(sample);

#ifdef ZTS
#define SAMPLE_G(v) TSRMG(sample_globals_id, zend_sample_globals *, v)
#else
#define SAMPLE_G(v) (sample_globals.v)
#endif

#endif	/* PHP_SAMPLE_H */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
