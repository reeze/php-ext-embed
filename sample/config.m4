dnl $Id$
dnl config.m4 for extension sample

dnl Add this at the top of your extension's config.m4 file
m4_include(php-ext-embed/php_ext_embed.m4)

PHP_ARG_WITH(sample, for sample support,
[  --enable-sample             	Include sample support]
[  --disable-sample-embed-lib   Disable sample embed lib])

if test "$PHP_SAMPLE" != "no"; then
  dnl check dependencies
  PHP_EXT_EMBED_INIT()
  
  PHP_SUBST(SAMPLE_SHARED_LIBADD)

  dnl php libs will be auto included in the order added
  dnl You could add the php files you want to embed to 
  dnl your extension
  PHP_EXT_EMBED_ADD_LIB(sample, [lib/*.php])

  dnl add php_ext_embed.c to build to support php lib embed
  dnl if you want to upgrade from your extension, please
  dnl replace PHP_NEW_EXTENSION by PHP_EXT_EMBED_NEW_EXTENSION
  PHP_EXT_EMBED_NEW_EXTENSION(sample, [sample.c], $ext_shared)
fi
