php-ext-embed
=============

Build Status: [![Build Status](https://secure.travis-ci.org/reeze/php-ext-embed.png)](http://travis-ci.org/reeze/php-ext-embed)

*Mix it up with PHP and C*

`php-ext-embed` is a tool which help you to build php extension
with both `PHP` and `C` by embed your php lib right inside your
extension.

## Why?

The PHP lib could be embeded into your single extension so file.
then your could write the performance ciritical part with `C` and the rest
with `PHP`, you got a single extension file without dependency.

## How?

1. Download php-ext-embed
1. generate your extension skeleton

```
$ cd php-ext-embed
$ bin/php-ext-embed --ext=myext
$ cd myext
$ # add your logic
```

## Limitations

1. Lib files could only be in same folder for now
1. It works for \*Nix for now, Windows is not supported yet


