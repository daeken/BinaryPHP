<?php
$resource = socket_create(AF_INET, SOCK_STREAM, 0);

if(is_resource($resource))
	echo 'Thats right!', "\n";
?>
