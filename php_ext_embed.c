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

#include <php.h>
#include <main/php_main.h>

#include <Zend/zend_compile.h>
#include <Zend/zend_execute.h>

#include <Zend/zend.h>
#include <Zend/zend_ini.h>



#include "php_ext_embed.h"

#ifndef MAX_PATH_LEN
#define MAX_PATH_LEN 256
#endif

typedef struct _embed_info {
	FILE *fp;
	size_t offset;
	size_t length;
} ext_embed_info;

static ext_embed_info get_ext_embed_info(char *so_path, php_ext_lib_entry *entry)
{
	ext_embed_info info = {0};
#ifdef __APPLE__
   const struct section_64 *sect = getsectbyname("__text", entry->section_name);
   if (sect) {
     info.offset = sect->offset;
     info.length = sect->size;
   }
#else
# error Linux is  Not implemented yet
#endif

     return info;
}


#define ENTRY_FOREACH(embed_files, entry) \
	for (entry = embed_files; entry->filename != NULL; (embed_files++), entry = embed_files)

static zend_file_handle* get_embed_file_handle(const char *extname, php_ext_lib_entry *entry TSRMLS_DC)
{
	static char so_path[MAX_PATH_LEN] = {0};

	// TODO support static extension build
	if (so_path[0] == '\0') {
		snprintf(so_path, MAX_PATH_LEN, "%s/%s.so", INI_STR("extension_dir"), extname);
	}

	if (!entry->handle) {
		ext_embed_info info = get_ext_embed_info(so_path, entry);
		zend_file_handle *handle = pemalloc(sizeof(zend_file_handle), 1);
		/* Use mode 0 for now */
		int result = php_stream_open_for_zend_ex(so_path, handle, 0 TSRMLS_CC);

/*
		handle.type = ZEND_HANDLE_MAPPED;
		handle->filename = entry->dummy_filename;
		handle.stream.mmap.buf = handle.fp;
		*/

		entry->handle = handle;
	}

	return entry->handle;
}

int php_embed_startup(const char *extname, php_ext_lib_entry *embed_files TSRMLS_DC)
{
	php_ext_lib_entry *entry = NULL;

	ENTRY_FOREACH(embed_files, entry) {
	
	}
	return 0;
}

int php_embed_do_include_files(const char *extname, php_ext_lib_entry *embed_files TSRMLS_DC)
{
	php_ext_lib_entry *entry = NULL;

	ENTRY_FOREACH(embed_files, entry) {
		zval *retval = NULL;
		// TODO error checking?
		zend_execute_scripts(ZEND_INCLUDE TSRMLS_CC, &retval, 1,
			get_embed_file_handle(extname, entry TSRMLS_CC));
	}
	return 0;
}

int php_embed_shutdown(const char *extname, php_ext_lib_entry *embed_files TSRMLS_DC)
{
	php_ext_lib_entry *entry = NULL;

	ENTRY_FOREACH(embed_files, entry) {
	
	}

	return 0;
}

