php\_ext\_embed
=============

*Mix it up with PHP and C*

php\_ext\_embed is a tool which help you to build php extension
with both `PHP` and `C` by embed your php lib right inside your
extension.

## Why?

The PHP lib could be embeded into your single extension so file.
then your could write the performance ciritical part with `C` and the rest
with `PHP`, you got a single extension file without dependency.

## How?

1. Download php_ext_embed
1. generate your extension skeleton

```
$ cd php_ext_embed
$ bin/php_ext_embed_gen --ext=myext
$ cd myext
$ # add your logic
```

## Limitations

1. You could only embed *single* php file for now. multiple files support
coming soon.
1. It works for \*Nix for now, Windows is not supported yet


