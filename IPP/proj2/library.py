# File : library.py
# Author : Kristof Siska , xsiska16
# Project : interpret.py
# ------------------------------------- #
# This file contains classes and useful
# functions, which are used throughout 
# interpret project
# ------------------------------------- #

import sys
from operator import concat
import re

ERROR_FORMAT = 31
ERROR_STRUCTURE = 32

ERROR_SEMANTIC = 52
ERROR_OPPERAND_TYPES = 53
ERROR_UNDEFINED_VARIABLE = 54
ERROR_UNEXSTISTING_FRAME = 55
ERROR_MISSING_VALUE = 56
ERROR_BAD_OPERAND_VALUE = 57
ERROR_BAD_STRING_MANIPULATION = 58

# Class representing the arguent types of instructions
# Implements getter methods get_order, get_type, get_value
class Argument:
	def __init__(self, order, type, value):
		self.order = int(order)
		self.type = type
		self.value = value 
	
	def get_order(self):
		return self.order

	def get_type(self):
		return self.type
	
	def get_value(self):
		return self.value 

# Class Instruction is used to represent Instructions in IPPCode22
# After parsing the input xml file, store every instruction
# with its argument as Instruction instance. Working with
# instructions is made easier by working with Inst_list
class Instruction:
	Inst_list = []

	def __init__(self, opcode, order):
		self.opcode = opcode
		try:
			self.order = int(order)
		except Exception:
			fatal_error(ERROR_STRUCTURE, "Error : order must be int type")
		self.Arg_list = []
		self.Inst_list.append(self)
	
	def validate_order(self):
		if (self.order < 0):
			fatal_error(ERROR_STRUCTURE, "Error : order must be >= 0")

	def append_argument(self, arg):
		self.Arg_list.append(arg)

	def get_opcode(self):
		return self.opcode

	def get_order(self):
		return self.order

# Class Frame is used to handle Frame manipulation
# in IPPCode22. Frame can be manipulated with
# instructions like CreateFrame, PushFrame, Popframe
# Three main Frames are the Global Frame, Temporary Frame
# And Local Frame - which is always the top of Stack Frame
class Frame:
	
	def __init__(self):  # sourcery skip: dict-literal, merge-list-append
		self.temp_defined = False
		self.glob = dict()
		self.temp = dict()
		self.local = []
		self.local.append(dict())

	def get_top(self):
		top = len(self.local) - 1
		if top < 0:
			fatal_error(ERROR_UNEXSTISTING_FRAME, "Error : Frame stack is empty")
		return (len(self.local) - 1)

	def Createframe(self):
		if self.temp_defined == True:
			self.temp.clear()
		
		self.temp_defined = True
		self.temp = dict()
	
	def Pushframe(self):
		if self.temp_defined == False:
			fatal_error(ERROR_UNEXSTISTING_FRAME, "Error : No temporary frame")
		# TODO : Have to change TF@ into LF@
		transform_TF_to_LF(self)
		self.temp_defined = False

	def Popframe(self):
		if self.get_top() <= 0:
			fatal_error(ERROR_UNEXSTISTING_FRAME, "Error : No more local frames")
		transform_LF_to_TF(self)
		self.temp_defined = True
			
	def Defvar(self, name):
		top  = self.get_top()
		if (re.match(r'GF@[a-zA-Z_\-$&%*!?][a-zA-Z0-9_\-$&%*!?]*', name)):
			if (name not in self.glob.keys()):
				self.glob[name] = None
			else:
				fatal_error(ERROR_SEMANTIC, "Error : Var already defined")

		elif (re.match("LF@[a-zA-Z_\-$&%*!?][a-zA-Z0-9_\-$&%*!?]*", name)):
			if (name not in self.local[top].keys()):
				self.local[top][name] = None
			else:
				fatal_error(ERROR_SEMANTIC, "Error : Var already defined")
		
		elif (re.match("TF@[a-zA-Z_\-$&%*!?][a-zA-Z0-9_\-$&%*!?]*", name)):
			if self.temp_defined == False:
				fatal_error(ERROR_UNEXSTISTING_FRAME, "Error : Temp frame doesnt exist")
			if (name not in self.temp.keys()):
				self.temp[name] = None
			else:
				fatal_error(ERROR_SEMANTIC, "Error : Var already defined")

	def find_var(self,name):  # sourcery skip: de-morgan
		top = self.get_top()
		if not (name in self.glob.keys() or name in self.temp.keys() or name in self.local[top].keys()):
			fatal_error(ERROR_UNDEFINED_VARIABLE, "Error : Variable doesnt exist")
	
	def get_temp_var(self, name):
		if self.temp_defined == False:
			fatal_error(ERROR_UNEXSTISTING_FRAME, "Error : Temporary frame not existing")
		elif (name in self.temp.keys()):
			return self.temp[name]
		else:
			fatal_error(ERROR_UNDEFINED_VARIABLE, "Error : Var doesnt exist")

# Function for error handling.
# Prints the given error msg and
# Exits the program with given exit code. 
def fatal_error(code,msg):
	msg = concat(msg, "\n")
	sys.stderr.write(msg)
	exit(code)
	

# Function used in POPFRAME
# Transform every variable in local frame
# to temp frame variable. Changes the prefix
# 'LF@*' to 'TF@*'. Deletes the top of the
# Stack Frame
def transform_LF_to_TF(frames : Frame):
	top = frames.get_top()
	keys = frames.local[top].keys()
	tmp = dict()
	for key in keys:
		replace = key.replace("LF", "TF", 1)
		tmp[replace] = frames.local[top][key]
	frames.temp = tmp
	del frames.local[top]

# Function used in PUSHFRAME
# Transform every variable in temp frame
# to a local frame variable. Changes
# the prefix 'TF@*' to 'LF@*' and clears 
# temporary frame
def transform_TF_to_LF(frames : Frame):
	top = frames.get_top()
	keys = frames.temp.keys()
	tmp = dict()
	for key in keys:
		replace = key.replace("TF", "LF", 1)
		tmp[replace] = frames.temp[key]
	frames.local.append(tmp) 
	frames.temp.clear()

def check_num_args(expected, actual):
	if (expected != actual):
		fatal_error(ERROR_STRUCTURE, "Error : Wrong number of arguments")

# Function to print help on terminal if
# the argument -h or --help is given
def print_help():
	print("Usage : [-h|--help] [--source SOURCE] [--input INPUT]")
	print()
	print("    Options:")
	print("        -h --help")
	print("            Displays help on terminal")
	print("        --source SOURCE")
	print("            Expects the xml source file be given after --source tag")
	print("            If this tag is missing, expects xml file from stdin")
	print("        --input INPUT")
	print("            Expect the file for inputs to be given after --input tag")
	print("            If --input is missing expects input file from stdin")
	print("    At least one of --source or --input has to be given")
	exit(0)
