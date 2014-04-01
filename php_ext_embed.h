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

typedef struct php_ext_lib_entry {
	const char *filename;
	const char *hash;
} php_ext_lib_entry;

#define PHP_EXT_EMBED_MINIT(extname)			php_embed_startup(#extname, &ext_##extname##_embed_files)
#define PHP_EXT_EMBED_RINIT(extname)			php_embed_do_include_files(#extname, &ext_##extname##_embed_files)
#define PHP_EXT_EMBED_RSHUTDOWN(extname)
#define PHP_EXT_EMBED_MSTUTDOWN(extname)		php_embed_shutdown(#extname, &ext_##extname##_embed_files)

int php_embed_startup(const char *extname, const php_ext_lib_entry *embed_files);
int php_embed_do_include_files(const char *extname, const php_ext_lib_entry *embed_files);
int php_embed_shutdown(const char *extname, const php_ext_lib_entry *embed_files);

#endif
