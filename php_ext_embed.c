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

#include <libelf.h>
#include <gelf.h>

#ifdef __APPLE__
#include <mach-o/getsect.h>
#endif

#include <Zend/zend_compile.h>
#include <Zend/zend_execute.h>
#include <Zend/zend_stream.h>

#include "php_ext_embed.h"

typedef struct _embed_handle {
	FILE *fp;
	off_t offset;
	size_t length;
} ext_embed_handle;

/*
static zend_mmap get_embed_handle(char *extname)
{
	return {};
}

*/
#define ENTRY_FOREACH(embed_files, entry) \
	for (entry = embed_files; entry->filename != NULL; (embed_files++), entry = embed_files)

/*
static zend_file_handle get_embed_file_handle(char *extname)
{
	zend_file_handle handle = {0};
	embed_handle handle = get_embed_handle(extname);

	handle.type = ZEND_HANDLE_MAPPED;
	handle.filename = "ext-embed-dummy-file.php";
	handle.stream.mmap.buf = handle.fp;
	// TODO
	return 0;
}
	*/

int php_embed_startup(const char *extname, php_ext_lib_entry *embed_files)
{
	const php_ext_lib_entry *entry = NULL;

	ENTRY_FOREACH(embed_files, entry) {
	
	}
	return 0;
}

int php_embed_do_include_file(const char *extname, php_ext_lib_entry *embed_files)
{
	const php_ext_lib_entry *entry = NULL;

	ENTRY_FOREACH(embed_files, entry) {
	
	}
	return 0;
}

int php_embed_shutdown(const char *extname, php_ext_lib_entry *embed_files)
{
	return 0;
}

