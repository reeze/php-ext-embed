/*
   +----------------------------------------------------------------------+
   | PHP Embeddable Ext Project                                           |
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
   | Authors: Shen Jiale <jerryshen.zju@gmail.com>                        |
   +----------------------------------------------------------------------+
*/

#ifndef _PHP_EXT_EMBED_OPCODE_CACHE_H_
#define _PHP_EXT_EMBED_OPCODE_CACHE_H_

void php_embed_compile_string_init();
void php_embed_compile_string_finish();
void php_embed_cache_restore();

#endif
