php-ext-embed
=============

Build Status:
[![Build Status](https://secure.travis-ci.org/reeze/php-ext-embed.png)]
(http://travis-ci.org/reeze/php-ext-embed)


## What it is

**Mixed-language programming with PHP and C!**

php-ext-embed  is  a  tool  to  help  you  embed pieces of PHP script right into
your php extension (which produced as `*.so`, written in C).

## Why should I use

The  reason  why we are writing PHP extensions is simple - improving performance
by  implementing  critical  algorithms in C under the most of circumstances, but
around  which you need some business logic code to wrap it, which is always very
complicated using Zend APIs that make the green hands trapped.

Our  work  makes  the scripts of PHP lib implant into a specific extension file,
then  you  could  only  write the performance critical part with C, and the rest
with PHP. Finally, you got a single extension file without dependency.

It makes it much easier to maintain and distribute your extension.


## How to use

Please refer to
[sample](https://github.com/reeze/php-ext-embed/tree/master/sample)  to  have  a
quick view at our example extension.

### Quick start

1. Clone this project.
1. make.
1. Now  the  extension  is  built  at  sample/modules/sample.so, add an entry in
   `php.ini` and try it out.
1. Update scripts in sample/lib/\*.php files to test.

### Generate new extension from scratch

1. Download php-ext-embed
1. generate your extension skeleton

> The util script is not finished yet

```
$ cd php-ext-embed
$ bin/php-ext-embed new extension-name
$ cd myext
$ # add your logic
```

### Upgrade from your exist extension

Only 7 lines changing needed to your extension.

1. Copy folder of this project to your extension root path.
1. Add include instructions to your main file of extension at the top.

    ```
    #include "php_ext_embed.h"
    #include "php_ext_embed_libs.h"
    ```

1. Add a macro to your RINIT function, in which "sample" is your module's name.

    ```
    PHP_EXT_EMBED_RINIT(sample);
    ```

1. add m4 inclusion in your config.m4 file

    ```
    m4_include(php-ext-embed/php_ext_embed.m4)
    ```

You can also refer to
[sample.c](https://github.com/reeze/php-ext-embed/blob/master/sample/sample.c)
and
[config.m4](https://github.com/reeze/php-ext-embed/blob/master/sample/config.m4)
in sample/ directory for example.

A script to help you upgrade is coming soon.


## Dependency

1. libelf-dev on *Nix (Mac OS X didn't require it)

*Note*:  If you don't have permission to install libelf to the system, you could
compile one to your custom directory and try:

```
./configure --with-libelf=/path/to/your/libelf
```


## Support

### Operating systems

Tested in these operating systems.

1. RedHat 4+
2. Mac OS X 10.9+

Windows is not supported yet.

### PHP versions

PHP 5.2, 5.3, 5.4, 5.5, 5.6

PHP 5.1 and below are not supported.

HHVM is not supported (and not needed because of the language `Hack`).


## Limitations

1. Lib files are included by the order declared in config.m4
1. lib's will **NOT** execute at the request startup like HHVM does,
   it just import your classes and functions
