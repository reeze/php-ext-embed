--TEST--
Test Method
--FILE--
<?php
var_dump(Sample::showMe());
$rf = new ReflectionClass('Sample');
echo($rf->getFileName());
?>
--EXPECTF--
The Money!
NULL
extension-embed://sample/lib/sample.class.php
