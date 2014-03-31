dnl
dnl $Id$
dnl
dnl This file contains helper autoconf functions for php_ext_embed
dnl

dnl
dnl PHP_EXT_EMBED_INIT(extname)
dnl
dnl check dependencies
dnl
dnl "extname" is the name of the ext/ subdir where the extension resides.
AC_DEFUN([PHP_EXT_EMBED_INIT],[
  AC_MSG_CHECKING([whether PHP_EXT_EMBED_DIR is set])
  if test "$PHP_EXT_EMBED_DIR" = ""; then
    AC_MSG_ERROR([PHP_EXT_EMBED_DIR is not set])
  fi

  AC_MSG_CHECKING([whether php_ext_embed_dir is correct])
  if test -f "$PHP_EXT_EMBED_DIR/php_ext_embed.h"; then
    AC_MSG_RESULT([yes])
  else
    AC_MSG_ERROR([php_ext_embed.h is not exist])
  fi
 
  dnl TODO Checking libelf? and add libs
  AC_MSG_CHECKING([whether libelf is found])

  dnl TODO PHP_ADD_EXTENSION_DEP('?')
])

dnl
dnl PHP_EXT_EMBED_ADD_LIB(extname, sources)
dnl
dnl Includes php lib to extension
dnl
dnl "extname" is the name of the ext/ subdir where the extension resides.
dnl "sources" is a list of files relative to the subdir which need to be
dnl           embeded to extension
AC_DEFUN([PHP_EXT_EMBED_ADD_LIB],[
  ext_builddir=[]PHP_EXT_BUILDDIR($1)
  ext_srcdir=[]PHP_EXT_SRCDIR($1)

  echo TODO
  dnl PHP_ADD_SOURCES(PHP_EXT_DIR($1),$2,$ac_extra,)
])
