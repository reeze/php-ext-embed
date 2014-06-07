php-ext-embed
=============

Build Status:
[![Build Status](https://secure.travis-ci.org/reeze/php-ext-embed.png)]
(http://travis-ci.org/reeze/php-ext-embed)


## What it is

php-ext-embed is a tool to help you embed sections of PHP script right into your
php extension (which produced as `*.so` file, written in C).


## Why should I use it

**Mixed-language programming with PHP and C!**

The  reason  why we are writing PHP extensions is simple - improving performance
by  implementing  critical  algorithms in C under the most of circumstances, but
around  which you need some business logic code to wrap it, which is always very
complicated using Zend APIs that make the green hands trapped.

Our  work  let  you implant PHP lib into a specific extension file, then you can
only write the performance critical part with C, and the rest with PHP. Finally,
you  get  a  single extension `.so` file without any external phplib dependency,
and get rid of all troubles of version incompatibility.

It makes it much easier to maintain and distribute your extension.


## How to use

Please refer to
[sample](https://github.com/reeze/php-ext-embed/tree/master/sample)  to  have  a
quick view at our sample extension.

### Quick start

1. Clone this project.
1. make.
1. Now  the extension file is built at sample/modules/sample.so, add an entry in
   `php.ini` and try it out.
1. Update scripts in sample/lib/\*.php files and `make` to test.

### Upgrade from your exist extension

1. Copy folder of this project to your extension root path.
1. execute `php-ext-embed/bin/php-ext-embed upgrade`, it will update your
   files automaticly

Or you could add those lines to your files yourself.

You can refer to
[sample.c](https://github.com/reeze/php-ext-embed/blob/master/sample/sample.c)
and
[config.m4](https://github.com/reeze/php-ext-embed/blob/master/sample/config.m4)
in sample/ directory for example.

Or all in one diff: <https://github.com/reeze/php-ext-embed/commit/3d5a8bab58ffe2d10c465ba4a671e1b89be3b2f3>

1. Add include instructions to your main file of extension at the top.

    ```
    #include "php_ext_embed.h"
    #include "php_ext_embed_libs.h"
    ```

1. Add macros that we provide to your SAPI functions to hook each stage, where
   "sample" is your module's name.

    ```
    PHP_EXT_EMBED_MINIT(sample);
    PHP_EXT_EMBED_MSHUTDOWN(sample);
    PHP_EXT_EMBED_RINIT(sample);
    PHP_EXT_EMBED_RSHUTDOWN(sample);
    ```

1. add m4 inclusion in your config.m4 file

    ```
    m4_include(php-ext-embed/php_ext_embed.m4)
	PHP_EXT_EMBED_INIT(sample)
    ```


## Dependency

1. libelf-dev on *Nix (Mac OS X didn't require it)

*Note*:  If you don't have permission to install libelf to the system, you could
compile one to your custom directory and try:

```
./configure --with-libelf=/path/to/your/libelf
```


## Support

### Issues

If you encounter any problem, feel free to fill an issue <https://github.com/reeze/php-ext-embed/issues>.

### Operating systems

Tested in these operating systems.

1. RedHat 4+
1. Ubunut 12+
1. Mac OS X 10.9+

Windows is not supported yet.

### PHP versions

PHP 5.2, 5.3, 5.4, 5.5, 5.6.

PHP 5.1 and below are not supported.

HHVM is not supported (The original idea came from HHVM, it support embed lib already).


## Limitations

1. Lib files are included by the order declared in config.m4
1. lib's will **NOT** execute at the request startup like HHVM does,
   it just import your classes and functions
