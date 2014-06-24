/*
   +----------------------------------------------------------------------+
   | PHP Embedable Ext                                                    |
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
   | Authors: Reeze Xia <reeze@php.net>                                   |
   +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifndef _PHP_EXT_EMBED_H_
#define _PHP_EXT_EMBED_H_

#include <stdio.h>
#include <unistd.h>
#include <time.h>

#include <php.h>

#define PHP_EXT_EMBED_VERSION		"1.0.1"
#define PHP_EXT_EMBED_API_NO		10000

#define PHP_EXT_EMBED_PROTO_NAME	"extension-embed"

#if PHP_MAJOR_VERSION == 4 || (PHP_MAJOR_VERSION == 5 && PHP_MINOR_VERSION < 2)
#error "php_ext_embed only supports PHP 5.2 and higher"
#endif

#ifdef PHP_EXT_EMBED_DEBUG
#define EMBED_DBG(args...) php_error_docref(NULL TSRMLS_CC, E_WARNING, "DEBUG: " ##args)
#else
#define EMBED_DBG(args...)
#endif

/*
 * PHP-Ext-Embed register a internal php wrapper to
 * load php lib from binary. This make the libs can take
 * advantage of opcache extension, eg: apc, opcache...
 */
typedef struct _php_ext_embed_wrapper {
	php_stream_wrapper wraper;	/* should be the first */
	const char *extname;		/* for debugging */
	int api_no;					/* to detect future possible incompatible if we change api */
	HashTable embeded_entries;	/* used to improve speed for entry lookup */
} php_ext_embed_wrapper;

typedef struct php_ext_lib_stat {
	size_t offset;
	size_t size;
	time_t m_time;
} php_ext_lib_stat;

/*
 * An entry for a embeded php lib file
 */
typedef struct _php_ext_lib_entry {
	const char *extname;
	const char *filename;				/* relative filename */
	const char *dummy_filename;			/* internal filename */
	const char *section_name;			/* the section name of the embed file in so */
	int include_on_rinit;			 	/* it allow user embed but not include, yes for future */
	php_ext_lib_stat stat;				/* basic info for entry */
} php_ext_lib_entry;

/* 
 * the same naming conversion as your entension, please put the corresponding
 * macro to your extension
 */
#define PHP_EXT_EMBED_MINIT(extname)		php_embed_minit(#extname, ext_ ## extname ## _embed_files TSRMLS_CC)
#define PHP_EXT_EMBED_RINIT(extname)		php_embed_rinit(#extname, ext_ ## extname ## _embed_files TSRMLS_CC)
#define PHP_EXT_EMBED_RSHUTDOWN(extname)	php_embed_rshutdown(#extname, ext_ ## extname ## _embed_files TSRMLS_CC)
#define PHP_EXT_EMBED_MSHUTDOWN(extname)	php_embed_mshutdown(#extname, ext_ ## extname ## _embed_files TSRMLS_CC)

int php_embed_minit(const char *extname, php_ext_lib_entry *embed_files TSRMLS_DC);
int php_embed_rinit(const char *extname, php_ext_lib_entry *embed_files TSRMLS_DC);
int php_embed_rshutdown(const char *extname, php_ext_lib_entry *embed_files TSRMLS_DC);
int php_embed_mshutdown(const char *extname, php_ext_lib_entry *embed_files TSRMLS_DC);

#endif
