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

#define PHP_EXT_EMBED_VERSION "0.1.0"
#define PHP_EXT_EMBED_API_NO 100518

#if PHP_MAJOR_VERSION == 4 || (PHP_MAJOR_VERSION == 5 && PHP_MINOR_VERSION < 2)
#error "php_ext_embed only supports PHP 5.2 and higher"
#endif

#define ENTRY_FOREACH(embed_files, entry) \
	php_ext_lib_entry *entry = NULL;	\
	for (entry = embed_files; entry->filename != NULL; (embed_files++), entry = embed_files)

typedef struct _php_ext_embed_wrapper {
	php_stream_wrapper wraper;	/* should be the first */
	int api_no;					/* to detect future possible incompatible if we change api */
	HashTable embeded_entries;	/* used to improve speed for entry lookup */
} php_ext_embed_wrapper;

typedef struct php_ext_lib_stat {
	size_t offset;
	size_t size;
	time_t m_time;
} php_ext_lib_stat;

typedef struct _php_ext_lib_entry {
	const char *extname;
	const char *filename;
	const char *dummy_filename;
	const char *section_name;
	struct _php_ext_lib_entry *head; /* List head of current extension for iteration */
	HashTable  *embeded_entries;	 /* Global embeded file entry hastable for reference */
	php_ext_lib_stat info;
} php_ext_lib_entry;

#define PHP_EXT_EMBED_MINIT(extname)		php_embed_startup(#extname, ext_ ## extname ## _embed_files TSRMLS_CC)
#define PHP_EXT_EMBED_RINIT(extname)		php_embed_do_include_files(#extname, ext_ ## extname ## _embed_files TSRMLS_CC)
#define PHP_EXT_EMBED_RSHUTDOWN(extname)
#define PHP_EXT_EMBED_MSHUTDOWN(extname)	php_embed_shutdown(#extname, ext_ ## extname ## _embed_files TSRMLS_CC)

int php_embed_startup(const char *extname, php_ext_lib_entry *embed_files TSRMLS_DC);
int php_embed_do_include_files(const char *extname, php_ext_lib_entry *embed_files TSRMLS_DC);
int php_embed_shutdown(const char *extname, php_ext_lib_entry *embed_files TSRMLS_DC);

#endif
