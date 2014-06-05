--TEST--
Test extends should work
--FILE--
<?php
$sample = new SampleStd();
$sample->a = 10;
$sample->b = 20;
var_dump($sample);
?>
--EXPECTF--
object(SampleStd)#1 (2) {
  ["a"]=>
  int(10)
  ["b"]=>
  int(20)
}
