dnl $Id$
dnl config.m4 for extension sample

PHP_ARG_WITH(sample, for sample support,
[  --enable-sample             	Include sample support])

if test "$PHP_SAMPLE" != "no"; then
  PHP_SUBST(SAMPLE_SHARED_LIBADD)

  m4_include(php-ext-embed/php_ext_embed.m4)
  PHP_EXT_EMBED_INIT(sample)

  dnl you could update it for your own need
  PHP_EXT_EMBED_ADD_INCLUDE_LIB(sample, [lib/*.php])

  PHP_EXT_EMBED_NEW_EXTENSION(sample, [sample.c], $ext_shared)
fi
