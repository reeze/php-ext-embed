/*
   +----------------------------------------------------------------------+
   | PHP Embeddable Ext                                                   |
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

#include <php.h>
#include <Zend/zend_compile.h>

#include "php_ext_embed.h"

extern php_ext_embed_wrapper ext_embed_wrapper;
extern int php_ext_embed_init_entry(HashTable *embeded_entries, php_ext_lib_entry *entry);

static php_ext_embed_wrapper* find_registered_wrapper()
{
	const HashTable *wrappers = php_stream_get_url_stream_wrappers_hash_global();
	php_ext_embed_wrapper *wrapper = NULL;

	zend_hash_find(wrappers, (char *)PHP_EXT_EMBED_PROTO_NAME,
		sizeof(PHP_EXT_EMBED_PROTO_NAME), (void **)&wrapper);

	return wrapper;
}

/*
 * Register 'extension-embed' stream wrapper, then we could transpantly load php
 * lib file without need to mannually cache classes/functions to improve speed
 */
int php_embed_startup(const char *extname, php_ext_lib_entry *embed_files TSRMLS_DC)
{
	php_ext_embed_wrapper *wrapper;
	wrapper = (php_ext_embed_wrapper *)find_registered_wrapper();

	if (!wrapper) {
		wrapper = &ext_embed_wrapper;
		zend_hash_init(&wrapper->embeded_entries, 0, NULL, NULL, 1);
		int ret = php_register_url_stream_wrapper(PHP_EXT_EMBED_PROTO_NAME,
			(php_stream_wrapper *)&ext_embed_wrapper TSRMLS_CC);

		if (ret == FAILURE) {
			php_error_docref(NULL TSRMLS_CC, E_WARNING, "Failed to startup php-ext-embed:"
				"Failed to register extension-embed stream wrapper to PHP");
			return FAILURE;
		}
	}

	/* merge embeded libs to php global wrapper instance to speedup lookup */
	ENTRY_FOREACH(embed_files, entry) {
		if (php_ext_embed_init_entry(&wrapper->embeded_entries, entry) == SUCCESS) {
			zend_hash_add(&wrapper->embeded_entries, entry->dummy_filename,
					strlen(entry->dummy_filename) + 1, entry, sizeof(entry), NULL);
		} else {
			php_error_docref(NULL TSRMLS_CC, E_WARNING, "Failed to init lib: %s", entry->dummy_filename);
		}
	}

	return SUCCESS;
}

int php_embed_do_include_files(const char *extname, php_ext_lib_entry *embed_files TSRMLS_DC)
{
	ENTRY_FOREACH(embed_files, entry) {
		if (!entry->include_on_rinit) {
			continue;
		}

		zend_file_handle file_handle;

		file_handle.type = ZEND_HANDLE_FILENAME;
		file_handle.filename = entry->dummy_filename;
		file_handle.handle.fp = NULL;
		file_handle.free_filename = 0;
		file_handle.opened_path = NULL;

		/* We just compile it to import class & function for now */
		zend_op_array *op_array = zend_compile_file(&file_handle, ZEND_INCLUDE TSRMLS_CC);

		if (op_array != NULL) {
			destroy_op_array(op_array TSRMLS_CC);
			efree(op_array);
		}
	}

	return SUCCESS;
}

int php_embed_shutdown(const char *extname, php_ext_lib_entry *embed_files TSRMLS_DC)
{
	php_ext_embed_wrapper *wrapper;
	wrapper = (php_ext_embed_wrapper *)find_registered_wrapper();

	if (wrapper) {
		zend_hash_destroy(&wrapper->embeded_entries);
		php_unregister_url_stream_wrapper(PHP_EXT_EMBED_PROTO_NAME TSRMLS_CC);
	}

	return SUCCESS;
}

