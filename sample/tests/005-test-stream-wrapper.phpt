--TEST--
Test Strea wrapper registered
--FILE--
<?php
var_dump(in_array("extension-embed", stream_get_wrappers()));
?>
--EXPECTF--
bool(true)
