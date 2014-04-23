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

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#include <libelf.h>
#include <gelf.h>

#ifdef __APPLE__
#include <mach-o/getsect.h>
#include <mach-o/dyld.h>
#endif

#include <php.h>
#include <Zend/zend_compile.h>
#include <Zend/zend_ini.h>

#include "php_ext_embed.h"

#ifndef MAX_PATH_LEN
#define MAX_PATH_LEN 256
#endif

static int read_block_data(char *file, size_t offset, size_t length, char *buf)
{
	int fd = open(file, O_RDONLY);

	if (fd < 0) {
		return FAILURE;
	}

	lseek(fd, offset, SEEK_SET);
	read(fd, buf, length);

	return SUCCESS;
}

/* Mostly came from HHVM source */
static zval* get_embed_data(char *bin_path, php_ext_lib_entry *entry TSRMLS_DC)
{
	zval* code = NULL;
	size_t offset = 0, length = 0;

#ifdef __APPLE__
	int i, count = _dyld_image_count();
	for (i = 0; i < count; i++) {
#ifndef __LP64__
		const struct mach_header *head = (struct mach_header *)_dyld_get_image_header(i);
		const struct section *section = getsectbynamefromheader(head,"__text", entry->section_name);
#else
		const struct mach_header_64 *head = (struct mach_header_64 *)_dyld_get_image_header(i);
		const struct section_64 *section = getsectbynamefromheader_64(head,"__text", entry->section_name);
#endif

		if (section) {
			offset = section->offset;
			length = section->size;
			break; 
		}   
	}   

#else
	GElf_Shdr shdr;
	size_t shstrndx = -1;
	char *name;
	Elf_Scn *scn;

	if (elf_version(EV_CURRENT) == EV_NONE) {
		return NULL;
	}

	int fd = open(bin_path, O_RDONLY, 0);
	if (fd < 0) {
		return NULL;
	}

	Elf* e = elf_begin(fd, ELF_C_READ, NULL);
	if (e == NULL || elf_kind(e) != ELF_K_ELF) {
		close(fd);
		elf_end(e); 
		return NULL;
	}

#ifdef HAVE_ELF_GETSHDRSTRNDX
	int stat = elf_getshdrstrndx(e, &shstrndx);
#else
	int stat = elf_getshstrndx(e, &shstrndx);
#endif

	if (stat < 0 || shstrndx == -1) {
		close(fd);
		elf_end(e); 
		return NULL;
	}

	scn = NULL;
	while ((scn = elf_nextscn(e, scn)) != NULL) {
		if (gelf_getshdr(scn, &shdr) != &shdr ||
				!(name = elf_strptr(e, shstrndx , shdr.sh_name))) {
			close(fd);
			elf_end(e); 
			return NULL;
		}
		if (!strcmp(entry->section_name, name)) {
			GElf_Shdr ghdr;
			if (gelf_getshdr(scn, &ghdr) != &ghdr) {
				close(fd);
				elf_end(e); 
				return NULL;
			}
			offset = ghdr.sh_offset;
			length  = ghdr.sh_size;
			break;
		}
	}
#endif

	MAKE_STD_ZVAL(code);

	/* Trick way to pass code to zend_compile_string. Explict close php */
	char close_tag[] = "?>";
	int buf_size = length + sizeof(close_tag);
	char *buf = emalloc(buf_size);

	strcpy(buf, close_tag);
	read_block_data(bin_path, offset, length, buf + strlen(close_tag));
	buf[buf_size - 1] = '\0';

	ZVAL_STRINGL(code, buf, buf_size - 1, 0);

	return code;
}


#define ENTRY_FOREACH(embed_files, entry) \
	for (entry = embed_files; entry->filename != NULL; (embed_files++), entry = embed_files)

int php_embed_startup(const char *extname, php_ext_lib_entry *embed_files TSRMLS_DC)
{
	return SUCCESS;
}


int php_embed_do_include_files(const char *extname, php_ext_lib_entry *embed_files TSRMLS_DC)
{
	php_ext_lib_entry *entry = NULL;
	static char bin_path[MAX_PATH_LEN] = {0};

	if (bin_path[0] == '\0') {
#ifdef PHP_EXT_EMBED_SHARED
		snprintf(bin_path, MAX_PATH_LEN, "%s/%s.so", INI_STR("extension_dir"), extname);
#else
		/* Then it is a static build */
#  ifdef __linux__
		ssize_t count = readlink("/proc/self/exe", bin_path, MAX_PATH_LEN);
#  elif defined(__APPLE__)
		uint32_t size = sizeof(bin_path);
		uint32_t success = _NSGetExecutablePath(bin_path, &size);
#  endif
#endif
	}

	ENTRY_FOREACH(embed_files, entry) {
		zval *code = get_embed_data(bin_path, entry TSRMLS_CC);

		if (!code) {
			php_error_docref(NULL TSRMLS_CC, E_WARNING, "failed to load embed lib: %s", entry->dummy_filename);	
			return FAILURE;	
		}

		zend_op_array *op_array = zend_compile_string(code, (char *)entry->dummy_filename TSRMLS_CC);

		/* We Just compile it to import class & function for now */
		/* TODO save imported classes/functions to reduce compile every RINIT */
		destroy_op_array(op_array TSRMLS_CC);
		efree(op_array);
		zval_ptr_dtor(&code);
	}

	return SUCCESS;
}

int php_embed_shutdown(const char *extname, php_ext_lib_entry *embed_files TSRMLS_DC)
{
	php_ext_lib_entry *entry = NULL;

	ENTRY_FOREACH(embed_files, entry) {
	
	}

	return 0;
}

