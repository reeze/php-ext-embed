--TEST--
Embed success
--FILE--
<?php
var_dump(function_exists('sample_lib_func'));
var_dump(class_exists('Sample'));
--EXPECT--
bool(true)
bool(true)
