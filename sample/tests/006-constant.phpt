--TEST--
Test Constant defined in source code should not available
--FILE--
<?php
var_dump(defined('SAMPLE_CONST_1'));
?>
--EXPECTF--
bool(false)
