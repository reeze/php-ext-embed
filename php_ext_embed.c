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

#include <Zend/zend.h>

#include "php_ext_embed.h"

typedef struct _embed_handle {
	FILE *fp;
	size_t offset;
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

static char* get_ext_so_file_path(char *extname)
{

}

static zend_file_handle get_embed_file_handle(char *extname, php_ext_lib_entry *entry)
{
	if (!entry->handle) {
		zend_file_handle *handle = pemalloc(sizeof(zend_file_handle));
		/* Use mode 0 for now */
		int result = php_stream_open_for_zend_ex(get_ext_so_file_path(extname), handle, 0);

		handle.type = ZEND_HANDLE_MAPPED;
		handle->filename = entry->dummy_filename;
		handle.stream.mmap.buf = handle.fp;

		entry->handle = handle;
	}

	return entry->handle;
	// TODO
	return 0;

#ifdef __APPLE__
   const struct section_64 *sect = getsectbyname("__text", section);
   if (sect) {
     desc->m_filename = fname;
     desc->m_start = sect->offset;
     desc->m_len = sect->size;
     return !desc->m_filename.empty();
   }

#else
# error Linux is  Not implemented yet
#endif

}

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
		// TODO error checking?
		zend_execute_scripts(ZEND_INCLUDE TSRMLS_DC, 1, get_embed_handle(extname, entry));
	}
	return 0;
}

int php_embed_shutdown(const char *extname, php_ext_lib_entry *embed_files)
{
	return 0;
}

