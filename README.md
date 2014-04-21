php-ext-embed
=============

Build Status: [![Build Status](https://secure.travis-ci.org/reeze/php-ext-embed.png)](http://travis-ci.org/reeze/php-ext-embed)

*Mix it up with PHP and C*

`php-ext-embed` is a tool to help you build your php extension
with both `PHP` and `C` by embed your php lib right inside your
extension.

## Why?

The PHP lib could be embeded into your single extension so file.
then your could write the performance ciritical part with `C` and the rest
with `PHP`, you got a single extension file without dependency.

## How to use

Please refer to sample extension: <https://github.com/reeze/php-ext-embed/blob/master/sample>

Only 7 lines changing needed to your extension.

**Quick start: **

1. Download php-ext-embed
1. `$ cd php-ext-emebed`
1. `$ make # now you build you extension`
1. update sample/lib/*.php files to test

### New extension from scratch
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

1. PHP >= 5.2
1. libelf-dev on *Nix

## Limitations

1. Lib files are included by the order decleared in config.m4
1. lib's will not execute at the request startup like HHVM does,
   it just import your classes and functions
1. It works for \*Nix for now, Windows is not supported yet
