--TEST--
Embed success
--FILE--
<?php
var_dump(function_exits('sample_lib_func'));
var_dump(class_exits('Sample'));
--EXPECT--
bool(true)
bool(true)
