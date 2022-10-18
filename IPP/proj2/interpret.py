# File : interpret.py
# Author : Kristof Siska , xsiska16
# Project : interpret.py
# ----------------------------------- #
# Main file of the interpret project
# ----------------------------------- #

import re
import argparse
import xml.etree.ElementTree as xmlTree
import sys

from instructions import *
from library import *

if "-h" in sys.argv or "--help" in sys.argv:
	print_help()
arg = argparse.ArgumentParser()
arg.add_argument("--source", nargs=1)
arg.add_argument("--input", nargs=1)

args = vars(arg.parse_args())

source_file = sys.stdin
input_file = sys.stdin

if (args["source"] != None):
	source_file = args["source"][0]

if (args["input"] != None):
	input_file = args["input"][0]
	input_file = open(input_file)

if source_file == sys.stdin and input_file == sys.stdin:
	fatal_error(10, "Error : Input file or source file has to be given")

try:
	tree = xmlTree.parse(source_file)
except Exception:
	fatal_error(ERROR_FORMAT, "Error : Bad xml format")

root = tree.getroot()


c1: Instruction

if root.tag != "program":
	fatal_error(ERROR_STRUCTURE , "Error : Missing program tag in root xml element")
keys = root.attrib.keys()
if 'language' not in keys:
	fatal_error(ERROR_STRUCTURE,"Error : Missing language tag in root xml element")
elif (root.attrib['language'] != 'IPPcode22'):
	fatal_error( ERROR_STRUCTURE,"Error : Other value than .IPPcode22 of language tag is not supported")


# Extract data from xml file
# Create instruction list and its params
for child in root:
	if (child.tag != "instruction"):
		fatal_error(ERROR_STRUCTURE,"Error : Other value than .IPPcode22 of language tag is not supported")

	keys = child.attrib.keys()
	if 'opcode' not in keys or "order" not in keys:
		fatal_error(ERROR_STRUCTURE,"Error : Missing 'opcode' or 'order' key")

	c1 = Instruction(child.attrib['opcode'], child.attrib['order'])
	c1.validate_order()
	for argument in child:
		x = re.match("arg[123]", argument.tag)
		if x is None:
			fatal_error(ERROR_STRUCTURE ,"Error : Unknown tag")

		keys = argument.attrib.keys()
		if 'type' not in keys:
			fatal_error(ERROR_STRUCTURE, "Error : Missing type key\n")
		val = None
		if argument.attrib['type'] in ["string", "var"]:
			val = str(argument.text)
		elif argument.attrib['type'] == "int":
			if c1.get_opcode() == "READ" and argument.text is None:
				val = None
			else:
				try:
					val = int(argument.text)
				except Exception:
					fatal_error(ERROR_STRUCTURE, "Error : Wrong int sequence")
		elif argument.attrib['type'] == "bool":
			if (argument.text.lower() == "true"):
				val = True
			elif argument.text.lower() == "false":
				val = False
			else:
				fatal_error(ERROR_BAD_OPERAND_VALUE, "ERROR : Bool has to be true or false")
		else:
			val = str(argument.text)

		to_append = Argument(argument.tag[len(argument.tag) - 1], argument.attrib['type'], val)
		c1.append_argument(to_append)

# Sort a list of objects
# Use lambda function as list 
# has no attribute of order
instruction_list = c1.Inst_list
instruction_list.sort(key=lambda x: x.order)

# Get a new sorted list
inst = sorted(instruction_list, key= lambda x: x.order)
i = 0
j = 0

# Check for order duplicity
while i < len(inst):
	order = inst[i].get_order()
	if (order <= 0):
		fatal_error(ERROR_STRUCTURE, "Error : order less than 1 found")
	j = i + 1
	while j < len(inst):
		if (inst[j] != None):
			if (inst[j].get_order() == order):
				fatal_error(ERROR_STRUCTURE, "Error : Duplicate orders")
			if (inst[j].get_order() > order):
				break
			j += 1
	i += 1


var_list = dict()
inst_len = len(inst)
i = 0
frames = Frame()


val_stack = []
pos_stack = []
labels = []
while i < inst_len:

	# Prepare 
	instruction = inst[i]
	instruction_name = instruction.get_opcode()
	arg = instruction.Arg_list

	arg.sort(key=lambda x: x.order)
	arg = sorted(arg, key= lambda x: x.order)

	instruction_name = instruction_name.upper()

	# Switch
	# Find the corresponding instruction
	# and call its function
	if instruction is None:
		exit(0)
	if (instruction_name == "MOVE"):
		check_num_args(2, len(arg))
		exec_MOVE(arg[0].value, arg[1], frames)

	elif (instruction_name == "DEFVAR"): 
		check_num_args(1, len(arg))
		frames.Defvar(arg[0].value)

	elif (instruction_name == "CREATEFRAME"):
		frames.Createframe()

	elif (instruction_name == "PUSHFRAME"):
		frames.Pushframe()

	elif (instruction_name == "POPFRAME"):
		frames.Popframe()

	elif (instruction_name == "CALL"):
		check_num_args(1, len(arg))
		i = exec_CALL(i, pos_stack, inst, arg[0].value)
		continue

	elif (instruction_name == "RETURN"):
		i = exec_RETURN(pos_stack)
		continue

	elif (instruction_name == "PUSHS"):
		check_num_args(1, len(arg))
		exec_PUSHS(val_stack, arg[0], frames)

	elif (instruction_name == "POPS"):
		check_num_args(1, len(arg))
		exec_POPS(val_stack, arg[0].value, frames)

	elif (instruction_name == "SUB"):
		check_num_args(3, len(arg))
		exec_SUB(arg[0].value, arg[1], arg[2], frames)

	elif (instruction_name == "MUL"):
		check_num_args(3, len(arg))
		exec_MUL(arg[0].value, arg[1], arg[2], frames)

	elif (instruction_name == "IDIV"):
		check_num_args(3, len(arg))
		exec_IDIV(arg[0].value, arg[1], arg[2], frames)

	elif (instruction_name == "ADD"):
		check_num_args(3, len(arg))
		exec_ADD(arg[0].value, arg[1], arg[2], frames)

	elif (instruction_name == "LT"):
		check_num_args(3, len(arg))
		exec_LT(arg[0].value, arg[1], arg[2], frames)

	elif (instruction_name == "GT"):
		check_num_args(3, len(arg))
		exec_GT(arg[0].value, arg[1], arg[2], frames) 

	elif (instruction_name == "EQ"):
		check_num_args(3, len(arg))
		exec_EQ(arg[0].value, arg[1], arg[2], frames) 

	elif (instruction_name == "AND"):
		check_num_args(3, len(arg))
		exec_AND(arg[0].value, arg[1], arg[2], frames)

	elif (instruction_name == "OR"):
		check_num_args(3, len(arg))
		exec_OR(arg[0].value, arg[1], arg[2], frames)

	elif (instruction_name == "NOT"):
		check_num_args(2, len(arg))
		exec_NOT(arg[0].value, arg[1], frames)

	elif (instruction_name == "INT2CHAR"):
		check_num_args(2, len(arg))
		exec_INT2CHAR(arg[0].value, arg[1], frames)

	elif (instruction_name == "STR2INT"):
		check_num_args(3, len(arg))
		exec_NOT(arg[0].value, arg[1], arg[2], frames) 

	elif (instruction_name == "READ"):
		# TODO specify type tag processing in parsing
		check_num_args(2, len(arg))
		exec_READ(arg[0].value, arg[1], input_file, frames)

	elif (instruction_name == "WRITE"):
		check_num_args(1, len(arg))
		exec_WRITE(arg[0], frames)

	elif (instruction_name == "CONCAT"):
		check_num_args(3, len(arg))
		exec_CONCAT(arg[0].value, arg[1], arg[2], frames)

	elif (instruction_name == "STRLEN"):
		check_num_args(2, len(arg))
		exec_STRLEN(arg[0].value, arg[1], frames)  

	elif (instruction_name == "GETCHAR"):
		check_num_args(3, len(arg))
		exec_GETCHAR(arg[0].value, arg[1], arg[2], frames) 

	elif (instruction_name == "SETCHAR"):
		check_num_args(3, len(arg))
		exec_SETCHAR(arg[0].value, arg[1], arg[2], frames) 

	elif (instruction_name == "TYPE"):
		check_num_args(2, len(arg))
		exec_TYPE(arg[0].value, arg[1], frames) 

	elif (instruction_name == "LABEL"):
		labels.append(arg[0].value)

	elif (instruction_name == "JUMP"):
		check_num_args(1, len(arg))
		i = int(exec_JUMP(arg[0].value, inst))
		continue

	elif (instruction_name == "JUMPIFEQ"):
		check_num_args(3, len(arg))
		i = exec_JUMPIFEQ(arg[0].value, arg[1], arg[2], inst, i, frames)
		continue

	elif (instruction_name == "JUMPIFNEQ"):
		check_num_args(3, len(arg))
		i = exec_JUMPIFNEQ(arg[0].value, arg[1], arg[2], inst, i, frames)

	elif (instruction_name == "EXIT"):
		check_num_args(1, len(arg))
		exec_EXIT(arg[0], frames)

	elif (instruction_name == "DPRINT"):
		exec_DPRINT(arg[0], frames)

	elif instruction_name == "BREAK":
		exec_BREAK()

	else:
		fatal_error(ERROR_STRUCTURE, f"Error : Unknown opcode - {instruction_name}")


	i += 1

# Check for duplicit labels
for i in range(len(labels)):
	if labels[i] in labels[i+1:]:
		fatal_error(ERROR_SEMANTIC, "Error : duplicate label")


if input_file != sys.stdin:
	input_file.close()
	
exit(0)