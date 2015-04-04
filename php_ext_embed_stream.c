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

#include <fcntl.h>
#include <sys/time.h>

#ifdef __APPLE__
#include <mach-o/getsect.h>
#include <mach-o/dyld.h>
#else
#include <libelf.h>
#include <gelf.h>
#endif

#include <php.h>
#include <php_streams.h>
#include <Zend/zend_ini.h>

#include "php_ext_embed.h"

struct php_ext_embed_stream_data_t {
	php_ext_lib_entry *entry;
	size_t cursor;
};

php_stream *php_stream_ext_embed_opener(php_stream_wrapper *wrapper,
											char *path,
											char *mode,
											int options,
											char **opened_path,
											php_stream_context *context STREAMS_DC TSRMLS_DC);

static php_stream_wrapper_ops ext_embed_stream_wops = {
	php_stream_ext_embed_opener,
	NULL,	/* close */
	NULL,	/* fstat */
	NULL,	/* stat */
	NULL,	/* opendir */
	"extension embed wrapper",
	NULL,	/* unlink */
	NULL,	/* rename */
	NULL,	/* mkdir */
	NULL	/* rmdir */
};

php_ext_embed_wrapper ext_embed_wrapper = {
	{
		&ext_embed_stream_wops,
		NULL,
		0 /* is_url */
	},
	"", /* extname will be set after install */
	PHP_EXT_EMBED_API_NO
};

static php_ext_lib_entry* get_entry_from_path(HashTable *embeded_entries, const char *path)
{
	php_ext_lib_entry **entry = NULL;
	zend_hash_find(embeded_entries, (char *)path, strlen(path) + 1, (void **)&entry);

	return entry ? *entry : NULL;
}

static void get_bin_path(php_ext_lib_entry *entry, char *buf)
{
	// Only dynamic shared object supported for now
	// XXX it assume the extension path is ${extname}.so so
	// so you have to name your extension as the rule
	snprintf(buf, MAXPATHLEN, "%s/%s.so", INI_STR("extension_dir"), entry->extname);

	EMBED_DBG("entry path: %s", buf);
}

static ssize_t read_entry_data(php_ext_lib_entry *entry, size_t offset, char *buf, size_t size)
{
	char bin_path[MAXPATHLEN];
	int fd = -1;
	ssize_t nbytes = -1;

	size_t read_len = ((entry->stat.size - offset > size) ? size : (entry->stat.size - offset));

	get_bin_path(entry, bin_path);
	fd = open(bin_path, O_RDONLY);

	if (fd < 0) {
		return FAILURE;
	}

	lseek(fd, entry->stat.offset + offset, SEEK_SET);

	nbytes = read(fd, buf, read_len);
	close(fd);

	return nbytes;
}

int php_ext_embed_init_entry(HashTable *embeded_entries, php_ext_lib_entry *entry)
{
	char bin_path[MAXPATHLEN];
	struct timeval tv;
	get_bin_path(entry, bin_path);

	entry->stat.offset = 0;
	entry->stat.size = 0;

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
			entry->stat.offset = section->offset;
			entry->stat.size = section->size;
			goto section_found;
		}
	}

	return FAILURE;

#else
	GElf_Shdr shdr;
	size_t shstrndx = -1;
	char *name;
	Elf_Scn *scn;

	if (elf_version(EV_CURRENT) == EV_NONE) {
		return FAILURE;
	}

	int fd = open(bin_path, O_RDONLY, 0);
	if (fd < 0) {
		TSRMLS_FETCH();
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "failed to open so file: %s"
			" please make sure your so file named as ${extname}.so", bin_path);
		return FAILURE;
	}

	Elf* e = elf_begin(fd, ELF_C_READ, NULL);
	if (e == NULL || elf_kind(e) != ELF_K_ELF) {
		goto failed_and_clean;
	}

#ifdef HAVE_ELF_GETSHDRSTRNDX
	int stat = elf_getshdrstrndx(e, &shstrndx);
#else
	int stat = elf_getshstrndx(e, &shstrndx);
#endif

	if (stat < 0 || shstrndx == -1) {
		goto failed_and_clean;
	}

	scn = NULL;
	while ((scn = elf_nextscn(e, scn)) != NULL) {
		if (gelf_getshdr(scn, &shdr) != &shdr ||
				!(name = elf_strptr(e, shstrndx , shdr.sh_name))) {
			goto failed_and_clean;
		}
		if (!strcmp(entry->section_name, name)) {
			GElf_Shdr ghdr;
			if (gelf_getshdr(scn, &ghdr) != &ghdr) {
				goto failed_and_clean;
			}
			close(fd);
			elf_end(e);
			entry->stat.offset = ghdr.sh_offset;
			entry->stat.size = ghdr.sh_size;
			goto section_found;
		}
	}
failed_and_clean:
	close(fd);
	elf_end(e);
	return FAILURE;

#endif

section_found:

	/* Fake m_time since it wont change anyway */
	gettimeofday(&tv, NULL);
	entry->stat.m_time = tv.tv_sec;

	return SUCCESS;
}

#define STREAM_DATA_FROM_STREAM() \
	struct php_ext_embed_stream_data_t *self = (struct php_ext_embed_stream_data_t *) stream->abstract;

/* {{{ ext_embed_ops_read */
static size_t ext_embed_ops_read(php_stream *stream, char *buf, size_t count TSRMLS_DC)
{
	ssize_t n = 0;

	STREAM_DATA_FROM_STREAM();

	n = read_entry_data(self->entry, self->cursor, buf, count);

	if (n < 0) {
		return 0;
	}

	if (n == 0 || n < count || (n == self->entry->stat.size)) {
		stream->eof = 1;
	}

	self->cursor += n;

	return (n < 1 ? 0 : n);
}
/* }}} */

/* {{{ ext_embed_ops_write */
static size_t ext_embed_ops_write(php_stream *stream, const char *buf, size_t count TSRMLS_DC)
{
	// read only
	return 0;
}
/* }}} */

/* {{{ ext_embed_ops_close */
static int ext_embed_ops_close(php_stream *stream, int close_handle TSRMLS_DC)
{
	STREAM_DATA_FROM_STREAM();
	if (close_handle) {
	}
	efree(self);
	stream->abstract = NULL;
	return EOF;
}
/* }}} */

/* {{{ ext_embed_ops_flush */
static int ext_embed_ops_flush(php_stream *stream TSRMLS_DC)
{
	// read only
	return 0;
}
/* }}} */

static int ext_embed_ops_stat(php_stream *stream, php_stream_statbuf *ssb TSRMLS_DC) /* {{{ */
{
	const char *path = stream->orig_path;
	int path_len = strlen(stream->orig_path);

	STREAM_DATA_FROM_STREAM();

	path_len = strlen(path);
	if (path_len >= MAXPATHLEN) {
		return -1;
	}

	if (!self->entry) {
		return -1;
	}

	memset(ssb, 0, sizeof(php_stream_statbuf));
	if (path[path_len-1] != '/') {
		ssb->sb.st_size = self->entry->stat.size;
		ssb->sb.st_mode |= S_IFREG; /* regular file */
	} else {
		ssb->sb.st_size = 0;
		ssb->sb.st_mode |= S_IFDIR; /* regular directory */
	}

	ssb->sb.st_mtime = self->entry->stat.m_time;
	ssb->sb.st_atime = self->entry->stat.m_time;
	ssb->sb.st_ctime = self->entry->stat.m_time;
	ssb->sb.st_nlink = 1;
	ssb->sb.st_rdev = -1;
	ssb->sb.st_ino = -1;

	return 0;
}
/* }}} */

php_stream_ops php_stream_ext_embedio_ops = {
	ext_embed_ops_write, ext_embed_ops_read,
	ext_embed_ops_close, ext_embed_ops_flush,
	PHP_EXT_EMBED_PROTO_NAME,
	NULL, /* seek */
	NULL, /* cast */
	ext_embed_ops_stat, /* stat */
	NULL  /* set_option */
};

/* {{{ php_stream_ext_embed_opener */
php_stream *php_stream_ext_embed_opener(php_stream_wrapper *wrapper,
											char *path,
											char *mode,
											int options,
											char **opened_path,
											php_stream_context *context STREAMS_DC TSRMLS_DC)
{
	php_stream *stream = NULL;
	struct php_ext_embed_stream_data_t *self;
	php_ext_embed_wrapper *embed_wrapper = (php_ext_embed_wrapper *)wrapper;

	self = emalloc(sizeof(*self));
	self->cursor = 0;

	self->entry = get_entry_from_path(&embed_wrapper->embeded_entries, path);
	if (!self->entry) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Embeded lib file not found: %s", path);
		return NULL;
	}

	stream = php_stream_alloc(&php_stream_ext_embedio_ops, self, NULL, mode);

	if (opened_path) {
		*opened_path = estrdup(path);
	}

	return stream;
}
/* }}} */
