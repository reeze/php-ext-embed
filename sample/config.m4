dnl $Id$
dnl config.m4 for extension sample

m4_include(php-ext-embed/php_ext_embed.m4)

PHP_ARG_WITH(sample, for sample support,
[  --enable-sample             	Include sample support]
[  --disable-sample-embed-lib   Disable sample embed lib])

if test "$PHP_SAMPLE" != "no"; then
  dnl TODO add toggle to disable embed Lib
  dnl Init php ext embed
  PHP_EXT_EMBED_INIT()
  
  PHP_SUBST(SAMPLE_SHARED_LIBADD)

  dnl php libs will be auto included in the order added
  PHP_EXT_EMBED_ADD_LIB(sample, [lib/*.php])

  dnl add php_ext_embed.c to build to support php lib embed
  PHP_EXT_EMBED_NEW_EXTENSION(sample, [sample.c], $ext_shared)
fi
