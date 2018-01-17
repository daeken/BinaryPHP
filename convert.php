#!/usr/local/bin/php
<?php

	function showUsage()
	{
		echo 'Usage: convert.php --if <inputfile.php> --of <output> [-v | --verbose] [-h | --help]' , "\n";
	}
	$debug = false;
	/* If there are arguments, cycle through em */
	if ($_SERVER['argc'] > 4) { 
		for($i = 1; $i <= $_SERVER['argc']; ++$i) {
			if(@$_SERVER['argv'][$i] == '-v' OR @$_SERVER['argv'][$i] == '--verbose') {
				$debug = true;
			}
			if(@$_SERVER['argv'][$i] == '--if') {
				$input = '';
				$input = $_SERVER['argv'][$i + 1];
			}
			if(@$_SERVER['argv'][$i] == '--of') {
				$output = '';
				$output = $_SERVER['argv'][$i + 1];
				$outputfile = $output . '.cpp';
			}
			if(@$_SERVER['argv'][$i] == '--help' OR @$_SERVER['argv'][$i] == '-h') {
				showUsage();
			}
		}
	} else {
		showUsage();
		die ("Too Few Arguments\n");
	}
	
	if(!(isset($input) && strlen(trim($input)) && is_file($input))) {
		showUsage();
		die ($input . " isn't a file\n");
	}
	if(!(isset($output) && strlen(trim($output)))) { 
	    showUsage();
	    die ($output . "isn't a valid output file\n");
	}
	
	if($debug == true) {
		define('DEBUG', true);
	} else {
		define('DEBUG', false);
	}
	
	if (substr(PHP_OS, 0, 3) == 'WIN') {
		define('BINARYPHP_OS', 'Windows');
	} else {
		define('BINARYPHP_OS', 'Unix');
	}
	error_reporting(E_ALL);
	if(!empty($_SERVER['DOCUMENT_ROOT']))
		header('Content-Type: text/plain');
	require_once 'tokenizer.php';
	require_once 'tokenflow.php';
	require_once 'functions.php';
	$tokenizer = new Tokenizer();
	$tokenizer->Parse($input);
	$tokenizer->Strip();
	$gen = new Generator($tokenizer);
	list($code, $flags) = $gen->Convert();
	if(DEBUG)
		$flag = ' -g';
	else
		$flag = (string) null;
	$lines = explode("\n", $code);
	if(DEBUG == true) {
		foreach($lines as $line => $cod)
			echo $line + 1, ': ', $cod, "\n";
	}
	$fp = fopen($output . '.cpp', 'wt');
	fwrite($fp, $code);
	fclose($fp);
	echo "\n\n", 'Compiling:', "\n";
	touch('log');
	if(BINARYPHP_OS == 'Windows') {
		if(DEBUG==true)
			$opt='-debug';
		else
			$opt='';
		$comp = 'cl -c /nologo /MT /W3 /GX /FD -DWIN32 /Fo'.$output.'.obj ' . $outputfile . ' >> log';
		echo $comp, "\n\n";
		shell_exec($comp);
		if(!file_exists('php_var.obj')) {
			$comp = 'cl -c /nologo /MT /W3 /GX /FD -DWIN32 /Fophp_var.obj php_var.cpp >> log';
			`echo $comp >> log`;
			shell_exec($comp);
		}		
	
		$release = 'link php_var.obj ' .$output . '.obj -nologo ' .$opt .' -incremental:no -opt:ref '.$flags.' -out:' . $output . '.exe >> log';
		echo $release, "\n\n";
		shell_exec($release);
	} else {
		if(!file_exists('php_var.lo'))
		{
			$comp = 'g++ -c -o php_var.lo php_var.cpp ' . $flag . ' >> log 2>> log';
			`echo $comp >> log`;
			shell_exec($comp);
		}
		$comp = 'g++ -o ' . $output . ' ' .$outputfile. ' php_var.lo '. $flags . $flag .  ' >> log 2>> log';
		`echo $comp >> log`;
		shell_exec($comp);
	}
	echo implode(null, file('log'));
	unlink('log');
?>
