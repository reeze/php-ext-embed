dnl $Id$
dnl config.m4 for extension sample

PHP_ARG_WITH(sample, for sample support,
[  --enable-sample             	Include sample support])

if test "$PHP_SAMPLE" != "no"; then
  PHP_SUBST(SAMPLE_SHARED_LIBADD)

  PHP_NEW_EXTENSION(sample, [sample.c], $ext_shared)
fi
