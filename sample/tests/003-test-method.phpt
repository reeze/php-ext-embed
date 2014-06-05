--TEST--
Test Method
--FILE--
<?php
var_dump(Sample::showMe());
$rf = new ReflectionClass('Sample');
echo($rf->getFileName()) . "\n";

$sample = new Sample();
var_dump($sample);
?>
--EXPECTF--
The Money!
NULL
extension-embed://sample/lib/sample.class.php
object(Sample)#2 (0) {
}
