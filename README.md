php-ext-embed
=============

Build Status: [![Build Status](https://secure.travis-ci.org/reeze/php-ext-embed.png)](http://travis-ci.org/reeze/php-ext-embed)

## What it is

**Mixed-language programming with PHP and C!**

`php-ext-embed` is a tool to help you implant pieces of PHP script right into
your php extension (produced as `*.so`, written in C).

## Why should I use it?

The PHP lib could be embedded into your single extension so file.
then your could write the performance critical part with `C` and the rest
with `PHP`, you got a single extension file without dependency.

It will be much easier to maintain and distribute your extension.

## How to use

Please refer to sample extension: <https://github.com/reeze/php-ext-embed/blob/master/sample>

Only 7 lines changing needed to your extension.

*Quick start:*

1. Download php-ext-embed
1. `$ cd php-ext-emebed`
1. `$ make # now you build you extension`
1. update sample/lib/\*.php files to test

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

1. copy php-ext-embed folder to your extension root path
1. Add below to your main extension file at the begin of your *.c file

```
#include "php_ext_embed.h"
#include "ext_embed_libs.h"
```
    please refer to <https://github.com/reeze/php-ext-embed/blob/master/sample/sample.c> for example.

1. then add MACRO: `PHP_EXT_EMBED_RINIT(sample);` to your RINIT function
    please refer to <https://github.com/reeze/php-ext-embed/blob/master/sample/sample.c> for example.

1. add m4 inclusion in your config.m4 file
    please refer to <https://github.com/reeze/php-ext-embed/blob/master/sample/config.m4> for example.

> A script to help you upgrade is coming soon.

## Requirements/Dependency

1. PHP 5.2+
1. libelf-dev on *Nix (Mac OS X didn't require it)

> If you don't have permission to install libelf, you could compile one with your own
> and try: `./configure --with-libelf=/path/to/your/libelf`

## Limitations

1. Lib files are included by the order decleared in config.m4
1. lib's will **NOT** execute at the request startup like HHVM does,
   it just import your classes and functions
1. It only works for \*Nix for now, Windows is not supported yet
