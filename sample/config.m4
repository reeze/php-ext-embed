dnl $Id$
dnl config.m4 for extension sample

PHP_EXT_EMBED_DIR=../

m4_include(../php_ext_embed.m4)

PHP_ARG_WITH(sample, for sample support,
[  --enable-sample             	Include sample support]
[  --disable-sample-embed-lib   Disable sample embed lib])

if test "$PHP_SAMPLE" != "no"; then
  dnl TODO add toggle to disable embed Lib
  dnl Init php ext embed
  PHP_EXT_EMBED_INIT()
  PHP_ADD_INCLUDE($PHP_EXT_EMBED_DIR)
  
  PHP_SUBST(SAMPLE_SHARED_LIBADD)
  PHP_EXT_EMBED_ADD_LIB(sample, [lib/sample.php])
  PHP_NEW_EXTENSION(sample, [sample.c $PHP_EXT_EMBED_DIR/php_ext_embed.c], $ext_shared)
fi
