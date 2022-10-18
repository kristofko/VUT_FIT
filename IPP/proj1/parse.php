<?php

ini_set('display_errors', 'stderr');

$MISSING_HEADER = 21;
$UNKNOWN_OPCODE = 22;
$SYNTAX_ERROR = 23;

enum var_states {
	case expect_LTG;
	case expect_F;
	case expect_at;
	case var_name;
	case var_name_valid;
	case error;
}

enum symb_states {
	case expect_type;
	case expect_at;
	case expect_value;
	case expect_value_valid;
	case expect_at_int;
	case expect_value_int;
	case expect_value_valid_int;
	case error;
}

function check_num_of_explodes($actual, $expected, $opcode){
	global $SYNTAX_ERROR;
	if ($actual != $expected){
		fprintf(STDERR, "Error : wrong number of arguments in %s opcode\n", $opcode);
		fprintf(STDERR, "Expected : %d\nActual : %d\n", $expected, $actual);
		exit($SYNTAX_ERROR);
	}

	return;
}

function check_type($line, $xml, $arg){
	global $SYNTAX_ERROR;
	if (!strcmp($line, "string")){
	}
	else if (!strcmp($line, "int")){
	}
	else if (!strcmp($line, "bool")){
	}
	else if (!strcmp($line, "nil")){
	}
	
	else {
		fprintf(STDERR, "Error : Invalid <type> in READ opcode\n");
		exit($SYNTAX_ERROR);
	}
	$instruction = $xml->addChild($arg, $line);
	$instruction->addAttribute("type", "type");

	return;
}

function check_label($line, $xml, $arg){

	global $SYNTAX_ERROR;

	$i = 0;
	$len = strlen($line);

	while ($i < $len && $line[$i] != "\n" ){
		if ($line[$i] == '\\' ||  $line[$i] == '#' || ctype_space($line[$i])){
			$i++;
		}
		if(ctype_digit($line[$i]) || preg_match('/[_\-$&%*!?a-zA-z]/', $line[$i])){
			$i++;
		}
		else {
			fprintf(STDERR, "Error : Invalid <label> sequence\n");
			exit($SYNTAX_ERROR);
		}
	}
	$instruction = $xml->addChild($arg, $line);
	$instruction->addAttribute("type", "label");
}

// Check <symb> syntax
// Made with FSM
function check_symb($line, $xml, $arg){

	global $SYNTAX_ERROR;

	$state = symb_states::expect_type;
	$i = 0;
	$len = strlen($line);
	$type = "";
	$value = "";
	
	while (($i < $len) && (!ctype_space($line[$i]) && ($line[$i] != "\n"))){

		switch($state){
			case symb_states::expect_type:
				if (!strncmp($line, "string", strlen("string"))){
					$i = strlen("string") - 1;
					$type = "string";
					$state = symb_states::expect_at;
				}
				else if (!strncmp($line, "int", strlen("int"))){
					// Expect -int for checking number sequence validity
					$i = strlen("int") - 1;
					$type = "int";
					$state = symb_states::expect_at_int;
				}
				else if (!strncmp($line, "bool", strlen("bool"))){
					$i = strlen("bool") - 1;
					$type = "bool";
					$state = symb_states::expect_at;
				}
				else if (!strncmp($line, "nil", strlen("nil"))){
					$i = strlen("nil") - 1;
					$type = "nil";
					$state = symb_states::expect_at;
				}
				else {
					$state = symb_states::error;

				}
				break;
			
			case symb_states::expect_at:
				if ($line[$i] == '@'){
					if (!strncmp($line, "string", strlen("string"))){
						$state = symb_states::expect_value_valid;
						break;
					}
					else {
						$state = symb_states::expect_value;
					}
				}
				else {
					$state = symb_states::error;

				}
				break;
			
			case symb_states::expect_value:
				//  TODO, check printable chars and check if no other 
				//  characters can be represented in string/bool/nil
				if(ctype_print($line[$i]) || ctype_digit($line[$i])){
					$value .= $line[$i];
					$state = symb_states::expect_value_valid;
				}
				else {
					$state = symb_states::error;

				}
				break;

			case symb_states::expect_value_valid:
				if(ctype_print($line[$i]) || ctype_digit($line[$i])){
					$value .= $line[$i];
					$state = symb_states::expect_value_valid;
				}
				else {
					$state = symb_states::error;
				}
				break;
			
			case symb_states::expect_at_int:
				if ($line[$i] == '@'){
					$state = symb_states::expect_value_int;
				}
				else {
					$state = symb_states::error;

				}
				break;
			
			case symb_states::expect_value_int:
				if(ctype_digit($line[$i]) || $line[$i] == '-'){
					$value .= $line[$i];
					$state = symb_states::expect_value_valid_int;
				}
				else {
					$state = symb_states::error;

				}
				break;

			case symb_states::expect_value_valid_int:
				if(ctype_digit($line[$i])){
					$value .= $line[$i];
					$state = symb_states::expect_value_valid_int;
				}
				else {
					$state = symb_states::error;

				}
				break;
		}
		$i++;

	}

	// <Symb> can also be a <var> 
	if ($state == symb_states::error){
		check_var($line, $xml, $arg );
	}
	elseif ($state != symb_states::expect_value_valid_int && $state != symb_states::expect_value_valid){
		fprintf(STDERR, "Error : Syntax error");
		exit($SYNTAX_ERROR);
	}
	else {
		$instruction = $xml->addChild($arg, $value);
		$instruction->addAttribute("type", $type);
	}
	return;
} // End of check_symb function



function check_var($line, $xml, $arg){

	global $SYNTAX_ERROR;

	$state = var_states::expect_LTG; 
	$i = 0;
	$special_characters = array("_","-","$","%","&","*","!","\\", "?");
	$len = strlen($line);
	// Xml hanndlers for var
	$instruction = $xml->addChild($arg, $line);
	$instruction->addAttribute("type", "var");

	while($i < $len && $line[$i] != "\n") {
		switch($state){
			
			case var_states::expect_LTG:
				if ($line[$i] == 'L' || $line[$i] == 'T' || $line[$i] == 'G'){
					$state = var_states::expect_F;
				}
				else {
					$state = var_states::error;
				}
				break;

			case var_states::expect_F:
				if ($line[$i] == 'F'){
					$state = var_states:: expect_at;
				}
				else {
					$state = var_states::error;
				}
				break;

			case var_states::expect_at:
				if ($line[$i]== '@'){
					$state = var_states::var_name;
				}
				else {
					$state = var_states::error;
				}
				break;

			case var_states::var_name:
				if (ctype_alpha($line[$i]) || in_array($line[$i], $special_characters)){
					$state = var_states::var_name_valid;
				}
				else {
					$state = var_states::error;
				}
				break;
			
			case var_states::var_name_valid:
				if (ctype_alpha($line[$i]) || in_array($line[$i], $special_characters) || ctype_digit($line[$i])){
					$state = var_states::var_name_valid;
				}
				else {
					$state = var_states::error;
				}
				break;
		}
		if ($state == var_states::error){
			break;
		}
		$i++;
	}
	if ($state == var_states::error || $state != var_states::var_name_valid){
		fprintf(STDERR,"Error : Invalid <var> or <symb> sequence\n");
		exit($SYNTAX_ERROR);
	}

	return;
}

if ($argc > 2){
	fprintf(STDERR,"Error : Wrong number of arguments\n");
	exit(10);
}
elseif ($argc == 2){
	if ($argv[1] == "--help"){
		echo "Skript typu filter (parse.php v jazyku PHP 8.1)\n";
		echo "Načíta zo štandardného vstupu zdrojový kód v IPPcode22,\n";
		echo "skontroluje lexikálnu a syntaktickú správnosť kódu\n";
		echo "a vypíše na standardní výstup XML reprezentaciu programu\n\n";
		exit(0);
	}
} 


$header_set = false;
$order = 0;

$xml_header = '<?xml version="1.0" encoding="UTF-8"?><program/>';
$xml = new SimpleXMLElement($xml_header);
$xml->addAttribute("language", "IPPcode22");

// Start of parsing input file
while( ($line = fgets(STDIN)) != false) {
	// Prepare data
	// First get rid of comments
	$line = preg_replace('/#.*/', "", $line);
	if ($line == "\n"){
		continue;
	}

	$stripped = trim($line);
	// Strip line of multiple space substrings 
	$single_spaced = preg_replace('/\s+/', ' ', $stripped);
	// Data is prepared
	
	if ($single_spaced == ""){
		continue;
	}

	$split = explode(' ',  $single_spaced);
	$num_of_explodes = count($split);

	if ($header_set == false){
		if(!strcmp($split[0], ".IPPcode22")){
			check_num_of_explodes($num_of_explodes, 1, $split[0]);
			$header_set = true;
			continue;
		}
	}

	$order++;
	$split[0] = strtoupper($split[0]);

	$instruction = $xml->addChild("instruction");
	$instruction->addAttribute("order", $order);
	$instruction->addAttribute("opcode", $split[0]);

	switch($split[0]) {
		case "CREATEFRAME":
		case "PUSHFRAME":
		case "POPFRAME":
		case "RETURN":
		case "BREAK":
			check_num_of_explodes($num_of_explodes, 1, $split[0]);
			break;
		
		case "DEFVAR":
		case "POPS":
			check_num_of_explodes($num_of_explodes, 2, $split[0]);
			check_var($split[1], $instruction, "arg1");
			break;
		
		case "PUSHS":
		case "WRITE":
		case "EXIT":
		case "DPRINT":
			check_num_of_explodes($num_of_explodes, 2, $split[0]);
			check_symb($split[1], $instruction, "arg1");
			break;

		case "CALL":
		case "LABEL":
		case "JUMP":
			check_num_of_explodes($num_of_explodes, 2, $split[0]);
			check_label($split[1], $instruction, "arg1");
			break;
		
		case "MOVE":
		case "INT2CHAR":
		case "STRLEN":
		case "TYPE":
			check_num_of_explodes($num_of_explodes, 3, $split[0]);
			check_var($split[1], $instruction, "arg1");
			check_symb($split[2], $instruction, "arg2");
			break;
		case "READ":
			check_num_of_explodes($num_of_explodes, 3, $split[0]);
			check_var($split[1], $instruction, "arg1");
			check_type($split[2], $instruction, "arg2");
			break;
		case "NOT":
			check_num_of_explodes($num_of_explodes, 3, $split[0]);
			check_var($split[1], $instruction, "arg1");
			check_symb($split[2], $instruction, "arg2");
			break;
		case "ADD":
		case "SUB":
		case "MUL":
		case "IDIV":
		case "LT":
		case "GT":
		case "EQ":
		case "AND":
		case "OR":
		case "STRI2INT":
		case "CONCAT":
		case "GETCHAR":
		case "SETCHAR":
			check_num_of_explodes($num_of_explodes, 4, $split[0]);
			check_var($split[1], $instruction, "arg1");
			check_symb($split[2], $instruction, "arg2");
			check_symb($split[3], $instruction, "arg3");
			break;
		case "JUMPIFEQ":
		case "JUMPIFNEQ":
			check_num_of_explodes($num_of_explodes, 4, $split[0]);
			check_label($split[1], $instruction, "arg1");
			check_symb($split[2], $instruction, "arg2");
			check_symb($split[3], $instruction, "arg3");
			break;
		case "\n":
			break;
		default:
			fprintf(STDERR, "ERROR : Unknown opcode %s\n", $split[0]);
			exit($UNKNOWN_OPCODE);
	}
} // End of parsing input file

if ($header_set == false){
	fprintf(STDERR, "Error : header not found\n");
	exit($MISSING_HEADER);
}

// Print formatted xml doc
$xmlDocument = new DOMDocument("1.0", "UTF-8");
$xmlDocument->preserveWhiteSpace = false;
$xmlDocument->formatOutput = true;
$xmlDocument->loadXML($xml->asXML());
$print = $xmlDocument->saveXML();
echo $print;


?>