dnl $Id$
dnl config.m4 for extension sample

PHP_EXT_EMBED_DIR=`pwd`/..

sinclude($PHP_EXT_EMBED_DIR/php_ext_embed.m4)

PHP_ARG_WITH(sample, for php_ext_emebed sample support,
[  --enable-sample             	Include sample support]
[  --disable-sample-embed-lib   Disable sample embed lib]

if test "$PHP_SAMPLE" != "no"; then
  dnl TODO add toggle to disable embed Lib
  dnl Init php ext embed
  PHP_EXT_EMBED_INIT()
  
  PHP_SUBST(SAMPLE_SHARED_LIBADD)
  PHP_EXT_EMBED_NEW_EXTENSION(sample, sample.c, $ext_shared)
fi
