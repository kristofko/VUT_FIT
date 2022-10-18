# File : instructions.py
# Author : Kristof Siska , xsiska16
# Project : interpret.py
# --------------------------------------------- #
# This file contains definitions of functions
# which are used to represent the instructions
# of IppCode22
# --------------------------------------------- #

import re
import string

from library import *

# Function to check whether variable is defined in any frame
def check_var_defined(var, frames : Frame):
	top = frames.get_top()
	var = str(var)
	if (re.match(r'^(GF|TF|LF)@[a-zA-Z_\-$&%*!?][a-zA-Z0-9_\-$&%*!?]*', var)):
		if (re.match(r'GF@', var)):
			if var not in frames.glob.keys():
				fatal_error(ERROR_UNDEFINED_VARIABLE, "Error : variable is not defined")
		elif (re.match(r'TF@',var)):
			if frames.temp_defined == False:
				fatal_error(ERROR_UNEXSTISTING_FRAME, "Error : Temporary frame doesnt exist")
			elif var not in frames.temp.keys():
				fatal_error(ERROR_UNDEFINED_VARIABLE, "Error : variable is not defined")
		elif (re.match(r'LF@', var)):
			if var not in frames.local[frames.get_top()].keys():
				fatal_error(ERROR_UNDEFINED_VARIABLE, "Error : variable is not defined")
	else:
		fatal_error(ERROR_STRUCTURE, "Error : Wrong variable name")

# Function to get value of 'symb'
# Failure if unknown operand type, trying
# To access undeclared or undefined variable
def get_symb_value(arg, frames : Frame):
	if arg.type == "string":
		return str(arg.value)
	elif arg.type == "int":
		return int(arg.value)
	elif arg.type == "bool":
		bool(arg.value)
	elif arg.type == "nil":
		return str(arg.value)
	elif arg.type == "var":
		check_var_defined(arg.value, frames)
		pre = arg.value[:2]
		if pre == "GF":
			return frames.glob[arg.value]
		elif pre == "TF":
			return frames.get_temp_var(arg.value)
		else:
			# We know it is LF and we dont have to check
			return frames.local[frames.get_top()][arg.value]
	else:
		fatal_error(ERROR_OPPERAND_TYPES, "Error : Unknown operand type")

# Help function to reduce code redundancy
# Used in every exec_* function, where symb
# manipulation takes place
def check_value_initialized(val):
	if (val == None):
		fatal_error(ERROR_MISSING_VALUE, "Error : Unitialized variable manipulation")

# Function  to assign value into given variable
# Failure if given variable is not declared
def move_val_to_var(var_name, val, frames):
	var_name = str(var_name)
	check_var_defined(var_name, frames)
	pre = var_name[:2]
	if pre == "GF":
		frames.glob[var_name] = val
	elif pre == "TF":
		frames.temp[var_name] = val
	elif pre == "LF":
		frames.local[frames.get_top()][var_name] = val

# Function to check that 2 values are of the same type
def check_same_types(val1, val2):
	if (not (type(val1) is type(val2))):
		fatal_error(ERROR_BAD_OPERAND_VALUE, "Error : Operand are not the same type")

# Assign value from arg to var
def exec_MOVE(var, arg, frames):
	val = get_symb_value(arg, frames)
	check_var_defined(var, frames)
	check_value_initialized(val)
	move_val_to_var(var, val, frames)

# Execute function
# Search throguh list of instructions to find 
# function with given func_name.
# Returns position of given function
def exec_CALL(current_position, pos_list, inst_list, func_name) -> int:
	pos_list.append(current_position + 1)
	i = 0
	for inst in inst_list:
		if inst.get_opcode() == "LABEL":
			if inst.Arg_list[0].value == func_name:
				return i
		i += 1

# Return from Call stack
def exec_RETURN(pos_list) -> int:
	return pos_list.pop()

# Push value into stack
def exec_PUSHS(val_stack, arg, frames):
	val = get_symb_value(arg, frames)
	check_value_initialized(val)
	val_stack.append(val)

# Take the value from top of the stack
# and assign it to given var
def exec_POPS(val_stack, var, frames):
	check_var_defined(var, frames)
	if (len(val_stack) > 0):
		val = val_stack.pop()
		move_val_to_var(var, val, frames)
	else:
		fatal_error(ERROR_MISSING_VALUE, "Error : Stack is empty")
	
# Add 2 ints and assign the result to var
# Failure if one of the values is not int
def exec_ADD(var, arg1, arg2, frames):
	val1 = get_symb_value(arg1, frames)
	val2 = get_symb_value(arg2, frames)
	check_value_initialized(val1)
	check_value_initialized(val2)
	if (type(val1) is not int or type(val2) is not int):
		fatal_error(ERROR_OPPERAND_TYPES, "Error : Both <symb> must be int in ADD instruction")
	else:
		move_val_to_var(var, int(val1 + val2), frames)

# Sub 1 int from another and assign the result to var
# Failure if one of the values is not int
def exec_SUB(var, arg1, arg2, frames):
	val1 = get_symb_value(arg1, frames)
	val2 = get_symb_value(arg2, frames)
	check_value_initialized(val1)
	check_value_initialized(val2)
	if (type(val1) is not int or type(val2) is not int):
		fatal_error(ERROR_OPPERAND_TYPES, "Error : Both <symb> must be int in ADD instruction")
	else:
		move_val_to_var(var, int(val1 - val2), frames)

# Multiply 2 ints and assign the result to var
# Failure if one of the values is not int
def exec_MUL(var, arg1, arg2, frames):
	val1 = get_symb_value(arg1, frames)
	val2 = get_symb_value(arg2, frames)
	check_value_initialized(val1)
	check_value_initialized(val2)
	if (type(val1) is not int or type(val2) is not int):
		fatal_error(ERROR_OPPERAND_TYPES, "Error : Both <symb> must be int in ADD instruction")
	else:
		move_val_to_var(var, int(val1 * val2), frames)

# Divide 1 int with another and assign the result to var
# Failure if one of the values is not int or the divider is 0.
def exec_IDIV(var, arg1, arg2, frames):
	val1 = get_symb_value(arg1, frames)
	val2 = get_symb_value(arg2, frames)
	check_value_initialized(val1)
	check_value_initialized(val2)
	if (type(val1) is not int or type(val2) is not int):
		fatal_error(ERROR_OPPERAND_TYPES, "Error : Both <symb> must be int in ADD instruction")
	elif (val2 == 0):
		fatal_error(ERROR_BAD_OPERAND_VALUE, "Error : Divison by 0")
	else:
		move_val_to_var(var, int(val1 // val2), frames)

# Start of comparation functions
# ------------------------------------------#
# Next line are valid for all comparation functions
# Comparation function arg1 < arg2
# Strings are compared lexicographically
# False is < True
# Return true or false
# Failure if values are not the same type

# arg1 < arg2
def exec_LT(var, arg1, arg2, frames):
	val1 = get_symb_value(arg1, frames)
	val2 = get_symb_value(arg2, frames)
	check_value_initialized(val1)
	check_value_initialized(val2)
	check_same_types(val1, val2)
	# We now know that both are the same type
	if(val1 < val2):
		move_val_to_var(var, bool(True), frames)
	else:
		move_val_to_var(var, bool(False), frames)

# arg1 > arg2
def exec_GT(var, arg1, arg2, frames):
	val1 = get_symb_value(arg1, frames)
	val2 = get_symb_value(arg2, frames)
	check_value_initialized(val1)
	check_value_initialized(val2)
	check_same_types(val1, val2)
	# We now know that both are the same type
	if(val1 > val2):
		move_val_to_var(var, bool(True), frames)
	else:
		move_val_to_var(var, bool(False), frames)

# arg1 == arg2
def exec_EQ(var, arg1, arg2, frames) -> bool:
	val1 = get_symb_value(arg1, frames)
	val2 = get_symb_value(arg2, frames)
	check_value_initialized(val1)
	check_value_initialized(val2)
	check_same_types(val1, val2)
	# We now know that both are the same type
	if(val1 == val2):
		move_val_to_var(var, bool(True), frames)
	else:
		move_val_to_var(var, bool(False), frames)

# arg1 and arg2 (must be bool)
def exec_AND(var, arg1, arg2, frames):
	val1 = get_symb_value(arg1, frames)
	val2 = get_symb_value(arg2, frames)
	check_value_initialized(val1)
	check_value_initialized(val2)
	if (type(val1) != bool or type(val2) != bool):
		fatal_error(ERROR_OPPERAND_TYPES, "Error : Incorrect types in AND opcode")
	else:
		to_write = (val1 and val2)
		move_val_to_var(var, bool(to_write), frames)


# arg1 or arg2 (must be bool)
def exec_OR(var, arg1, arg2, frames):
	val1 = get_symb_value(arg1, frames)
	val2 = get_symb_value(arg2, frames)
	check_value_initialized(val1)
	check_value_initialized(val2)
	if (type(val1) != bool or type(val2) != bool):
		fatal_error(ERROR_OPPERAND_TYPES, "Error : Incorrect types in OR opcode")
	else:
		to_write = (val1 or val2)
		move_val_to_var(var, bool(to_write), frames)

# not arg1 (must be bool)
def exec_NOT(var, arg1, frames):
	val1 = get_symb_value(arg1, frames)
	check_value_initialized(val1)
	if (type(val1) != bool):
		fatal_error(ERROR_OPPERAND_TYPES, "Error : Incorrect types in NOT opcode")
	else:
		to_write = not(val1)
		move_val_to_var(var, bool(to_write), frames)

# -----------------------------------------------#
# End of comparation functions

# Print value to output stream
def exec_WRITE(arg1, frames):
	val = get_symb_value(arg1, frames)
	if (type(val) == bool):
		if (val == True):
			print("true", end='')
		else:
			print("false", end='')
	else:
		if val == "nil":
			print("", end='')
		else:
			try:
				output = re.sub(r'\\([0-9]{3})', lambda a: chr(int(a[1])), val)
				print(output, end='')
			except(ValueError):
				print(val, end='')
			except(TypeError):
				print(val, end='')

# Convert integer to character and store
# it in var
def exec_INT2CHAR(var, arg1, frames):
	val = get_symb_value(arg1, frames)
	check_value_initialized(val)
	if (type(val) == str or type(val) == int):
		try:
			to_write = chr(int(val))
			move_val_to_var(var ,to_write, frames)
		except:
			fatal_error(ERROR_BAD_STRING_MANIPULATION, "Error : Bad int2char value")
	else:
		fatal_error(ERROR_OPPERAND_TYPES, "Error : INT2Char op type must be int or string")

# Convert Character in string on position arg2 to int
# Store it var
def exec_STR2INT(var, arg1, arg2 ,frames):
	val1 = get_symb_value(arg1)
	val2 = get_symb_value(arg2)
	check_value_initialized(val1)
	check_value_initialized(val2)
	check_var_defined(var, frames)
	if (type(val1) != str):
		fatal_error(ERROR_OPPERAND_TYPES, "Error : 1st operand in STR2INT must be string")
	elif (type(val2) != int):
		fatal_error(ERROR_OPPERAND_TYPES, "Error : 2nd operand in STR2INT must be int ")
	elif (val2 > len(val1) - 1 or val2 < 0):
		fatal_error(ERROR_BAD_STRING_MANIPULATION, "Error : Index out of bounds")
	else:
		to_move = ord(val1, val2)
		move_val_to_var(var, to_move, frames)

# Prompt user to insert an input
# Store the input as value in var
def exec_READ(var, arg1, input_file, frames):
	# arg1 is string from {int, string, bool}
	tmp :None
	if input_file == sys.stdin:
		tmp = input()
	else:
		# Input file is specified
		tmp = input_file.readline()
	tmp.strip()
	if arg1.value == "int":
		try:
			to_write = int(tmp)
		except(TypeError, ValueError):
			fatal_error(ERROR_SEMANTIC, "Error : Wrong int sequence")
		move_val_to_var(var, to_write, frames)
	if arg1.value == "string":
		to_write = str(tmp)
		move_val_to_var(var, to_write, frames)

	if arg1.value == "bool": 
		if (tmp.lower() == "true"):
			move_val_to_var(var, bool(True), frames)
		else:
			move_val_to_var(var, bool(False), frames)

# Concatenate 2 strings together
# Store the result in var
def exec_CONCAT(var, arg1, arg2, frames):
	val1 = get_symb_value(arg1, frames)
	val2 = get_symb_value(arg2, frames)
	check_value_initialized(val1)
	check_value_initialized(val2)
	if (type(val1) != str or type(val2) != str):
		fatal_error(ERROR_OPPERAND_TYPES, "Error : Only string op types allowed in concat")
	tmp = concat(val1, val2)
	move_val_to_var(var, str(tmp), frames)

# Get the length of a string
# Store the result in var
def exec_STRLEN(var, arg1, frames):
	val1 = get_symb_value(arg1)
	check_value_initialized(val1)
	if (type(val1) != str):
		fatal_error(ERROR_OPPERAND_TYPES, "Error : Only string op type allowed in strlen")
	tmp = len(val1)
	move_val_to_var(var, int(tmp), frames)

# Get the character in string on arg2 index
# Store the character as string value in var
def exec_GETCHAR(var, arg1, arg2, frames):
	val1 = get_symb_value(arg1, frames)
	val2 = get_symb_value(arg2, frames)
	check_value_initialized(val1)
	check_value_initialized(val2)
	if (type(val1) != str):
		fatal_error(ERROR_OPPERAND_TYPES, "Error : Only string op types allowed in getchar (1st)")
	if (type(val2) != int):
		fatal_error(ERROR_OPPERAND_TYPES, "Error : Only int op type allowed in getchar (2nd)")
	if (val2 > len(val1)):
		fatal_error(ERROR_BAD_STRING_MANIPULATION, "Error : Index out of bounds")
	tmp = val1[val2]
	move_val_to_var(var, str(tmp), frames)

# Set character in string var on position arg1 
# To character given by int arg2
# Store the result in var
def exec_SETCHAR(var, arg1, arg2, frames):
	val0 = get_symb_value(var)
	val1 = get_symb_value(arg1, frames)
	val2 = get_symb_value(arg2, frames)
	check_value_initialized(val0)
	check_value_initialized(val1)
	check_value_initialized(val2)
	if (type(val0) != str):
		fatal_error(ERROR_OPPERAND_TYPES, "Error : Only string op type allowed in setchar (1st)")
	if (type(val1) != int):
		fatal_error(ERROR_OPPERAND_TYPES, "Error : Only int op type allowed in setchar (2nd)")
	if (type(val2) != int):
		fatal_error(ERROR_OPPERAND_TYPES, "Error : Only int op type allowed in setchar (3rd)")

	if (val2 > len(val0) or val2 < 0):
		fatal_error(ERROR_BAD_STRING_MANIPULATION, "Error : Wrong string index")

	val0[val1] = chr(val2)
	
	move_val_to_var(var, str(val0), frames)

# Jump on a given label
# Loop throught the instruction list,
# if the Label is found, return its position + 1
# else Failure
def exec_JUMP(label_name, inst_list) -> int:
	i = int(0)
	while i < len(inst_list):
		if (inst_list[i].opcode == "LABEL"):
			arg = inst_list[i].Arg_list
			if (label_name == arg[0].value):
				return int(i + 1)
		i += 1
	
	# TODO check ret val
	fatal_error(ERROR_SEMANTIC, "Error : Label doesnt exist")

# Execute Jump if arg1 and arg2 values are equal
def exec_JUMPIFEQ(var, arg1, arg2, inst_list,curr_pos , frames) -> int:
	val1 = get_symb_value(arg1, frames)
	val2 = get_symb_value(arg2, frames)
	check_value_initialized(val1)
	check_value_initialized(val2)
	check_same_types(val1, val2)
	# We now know that both are the same type
	if(val1 == val2):
		i = int(exec_JUMP(var, inst_list))
		return i
	else:
		return curr_pos + 1

# Execute Jump if arg1 and arg2 values are not equal
def exec_JUMPIFNEQ(var, arg1, arg2, inst_list,curr_pos , frames) -> int:
	val1 = get_symb_value(arg1, frames)
	val2 = get_symb_value(arg2, frames)
	check_value_initialized(val1)
	check_value_initialized(val2)
	check_same_types(val1, val2)
	# We now know that both are the same type
	if(val1 != val2):
		i = int(exec_JUMP(var, inst_list))
		return i
	else:
		return curr_pos + 1

# Exit the program with a given exit code
# Exit code must be int type, otherwise failure
# Exit code must be in range <0, 49>, otherwise failure
def exec_EXIT(arg1, frames):
	val1 = get_symb_value(arg1, frames)
	check_value_initialized(val1)
	if (type(val1) != int):
		fatal_error(ERROR_OPPERAND_TYPES, "Error : Exit code has to be int type")
	if (not (val1 >= 0 and val1 <= 49)):
		fatal_error(ERROR_BAD_OPERAND_VALUE, "Error : Exit code value can be only <0, 49>")
	exit(val1)

# Get the type of arg2
# Store the type in var
def exec_TYPE(var, arg2, frames):
	val = get_symb_value(arg2, frames)
	val_type = type(val)
	if val_type == int:
		val_type = "int"
	elif val_type == string:
		val_type = "string"
	elif val_type == bool:
		val_type = "bool"
	elif val == None:
		val_type = ""
	move_val_to_var(var, val_type, frames)

def exec_DPRINT(arg1, frames):
	sys.stderr.write(get_symb_value(arg1, frames))

def exec_BREAK():
	sys.stderr.write("Pozicia v kode: Sme na instrukcii break")