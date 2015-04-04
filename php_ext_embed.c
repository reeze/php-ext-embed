/*
   +----------------------------------------------------------------------+
   | PHP Embeddable Ext                                                   |
   +----------------------------------------------------------------------+
   | Copyright (c) 1997-2015 The PHP Group                                |
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

#include <php.h>
#include <Zend/zend_compile.h>

#include "php_ext_embed.h"

#define ENTRY_FOREACH(embed_files, entry) do { \
	php_ext_lib_entry *entry = NULL; \
	for (entry = embed_files; entry->filename != NULL; (embed_files++), entry = embed_files)

#define END_ENTRY_FOREACH() } while(0)

extern php_ext_embed_wrapper ext_embed_wrapper;
extern int php_ext_embed_init_entry(HashTable *embeded_entries, php_ext_lib_entry *entry);

static php_ext_embed_wrapper* find_registered_wrapper()
{
	const HashTable *wrappers = php_stream_get_url_stream_wrappers_hash_global();
	php_ext_embed_wrapper **wrapperpp = NULL;

	zend_hash_find(wrappers, (char *)PHP_EXT_EMBED_PROTO_NAME,
		sizeof(PHP_EXT_EMBED_PROTO_NAME), (void **)&wrapperpp);

	return wrapperpp ? *wrapperpp : NULL;
}

/*
 * Register 'extension-embed' stream wrapper, then we could transpantly load php
 * lib file without need to mannually cache classes/functions to improve speed
 *
 * FIXME opcode cache: some version of eacc/opcache only cache stream `file://`
 *                     we use a custom stream, so the opcache are not work,
 *                     but you can still use it, or you could enable it by update eacc/opcache
 */
int php_embed_minit(const char *extname, php_ext_lib_entry *embed_files TSRMLS_DC)
{
	php_ext_embed_wrapper *wrapper;
	wrapper = (php_ext_embed_wrapper *)find_registered_wrapper();

	if (!wrapper) {
		wrapper = &ext_embed_wrapper;
		wrapper->extname = extname;
		zend_hash_init(&wrapper->embeded_entries, 8, NULL, NULL, 1);
		int ret = php_register_url_stream_wrapper(PHP_EXT_EMBED_PROTO_NAME,
			(php_stream_wrapper *)wrapper TSRMLS_CC);

		if (ret == FAILURE) {
			php_error_docref(NULL TSRMLS_CC, E_WARNING, "Failed to startup php-ext-embed: "
				"Failed to register extension-embed stream wrapper to PHP");
			return FAILURE;
		}
	} else {
		php_error_docref(NULL TSRMLS_CC, E_NOTICE, PHP_EXT_EMBED_PROTO_NAME
			" wrapper have been registered by extension: %s. reuse it", extname);
	}

	/* merge embeded libs to php global wrapper instance to speedup lookup */
	ENTRY_FOREACH(embed_files, entry) {
		void *dummy = NULL;
		if (zend_hash_find(&wrapper->embeded_entries, (char *)entry->dummy_filename,
					strlen(entry->dummy_filename) + 1, &dummy) == SUCCESS) {
			entry->include_on_rinit = 0;
			php_error_docref(NULL TSRMLS_CC, E_WARNING,
				"Duplicate lib detected ignored, lib: %s", entry->dummy_filename);
			continue;
		}

		if (php_ext_embed_init_entry(&wrapper->embeded_entries, entry) == SUCCESS) {
			zend_hash_add(&wrapper->embeded_entries, entry->dummy_filename,
					strlen(entry->dummy_filename) + 1, &entry, sizeof(entry), NULL);
		} else {
			php_error_docref(NULL TSRMLS_CC, E_WARNING,
				"Failed to init lib: %s", entry->dummy_filename);
		}
	} END_ENTRY_FOREACH();

	return SUCCESS;
}

static int php_embed_do_include_files(const char *extname, php_ext_lib_entry *embed_files TSRMLS_DC)
{
	ENTRY_FOREACH(embed_files, entry) {
		if (!entry->include_on_rinit) {
			EMBED_DBG("ignore not include file: %s", entry->dummy_filename);
			continue;
		}

		EMBED_DBG("do include file: %s", entry->dummy_filename);

		zend_file_handle file_handle;

		file_handle.type = ZEND_HANDLE_FILENAME;
		file_handle.filename = entry->dummy_filename;
		file_handle.handle.fp = NULL;
		file_handle.free_filename = 0;
		file_handle.opened_path = NULL;

		/*
		 * We just compile it to import class & function for now, so this means your code
		 * will not get executed, you can only declear top level classes & functions.
		 */
		zend_op_array *op_array = zend_compile_file(&file_handle, ZEND_INCLUDE TSRMLS_CC);

		if (op_array != NULL) {
			destroy_op_array(op_array TSRMLS_CC);
			efree(op_array);
		}
	} END_ENTRY_FOREACH();

	return SUCCESS;
}

int php_embed_rinit(const char *extname, php_ext_lib_entry *embed_files TSRMLS_DC)
{
	return php_embed_do_include_files(extname, embed_files TSRMLS_CC);
}

int php_embed_rshutdown(const char *extname, php_ext_lib_entry *embed_files TSRMLS_DC)
{
	return SUCCESS;
}

int php_embed_mshutdown(const char *extname, php_ext_lib_entry *embed_files TSRMLS_DC)
{
	php_ext_embed_wrapper *wrapper;
	wrapper = (php_ext_embed_wrapper *)find_registered_wrapper();

	// cleanup by yourself
	if (wrapper && wrapper->extname && strcmp(extname, wrapper->extname) == 0) {
		EMBED_DBG("Unregister stream wrapper by :%s", extname);
		php_unregister_url_stream_wrapper(PHP_EXT_EMBED_PROTO_NAME TSRMLS_CC);
	} else {
		EMBED_DBG("extension: %s do not own the wrapper ignored", extname);
	}

	return SUCCESS;
}

