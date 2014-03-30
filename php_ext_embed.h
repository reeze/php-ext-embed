/*
   +----------------------------------------------------------------------+
   | PHP Embedable Ext                                                    |
   +----------------------------------------------------------------------+
   | Copyright (c) 1997-2013 The PHP Group                                |
   +----------------------------------------------------------------------+
   | This source file is subject to version 3.01 of the PHP license,      |
   | that is bundled with this package in the file LICENSE, and is        |
   | available through the world-wide-web at the following url:           |
   | http://www.php.net/license/3_01.txt                                  |
   | If you did not receive a copy of the PHP license and are unable to   |
   | obtain it through the world-wide-web, please send a note to          |
   | license@php.net so we can mail you a copy immediately.               |
   +----------------------------------------------------------------------+
   | Authors: Reeze Xia <reeze@php.net>                                   |
   +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifndef _PHP_EXT_EMBED_H_
#define _PHP_EXT_EMBED_H_

#include <Zend/zend_compile.h>
#include <Zend/zend_execute.h>

#define PHP_EXT_EMBED_STARTUP(extname)			php_embed_startup(##extname)
#define PHP_EXT_EMBED_INCLUDE_FILES(extname)	php_embed_do_include_files(##extname)
#define PHP_EXT_EMBED_STUTDOWN(extname)			php_embed_shutdown(##extname)

int php_embed_startup(const char *extname);
int php_embed_do_include_files(const char *extname);
int php_embed_shutdown(const char *extname);

#endif
