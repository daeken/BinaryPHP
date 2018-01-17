<?php
echo "array_reverse tests:\n\n";
echo "test with numericly indexed array\n";
echo "original array:\n";
$array1 = array( 'a', 'b', 'c', 'd' );
var_dump( $array1 );
echo "reversed array:\n";
$array1 = array_reverse( $array1 );
var_dump( $array1 );

echo "test with textual index array\n";
$array2 = array( 'a' => 1, 'b' => 2, 'c' => 3, 'd' => 4 );
echo "original array:\n";
$array1 = array( 'a', 'b', 'c', 'd' );
var_dump( $array1 );
echo "reversed array:\n";
$array1 = array_reverse( $array1 );
var_dump( $array1 );
?>