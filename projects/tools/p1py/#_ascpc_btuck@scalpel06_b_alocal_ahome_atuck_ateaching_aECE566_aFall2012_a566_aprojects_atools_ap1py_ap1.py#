#!/usr/bin/env python2

import sys
import ply.lex as lex
import ply.yacc as yacc

# Import the llvmpy modules.
from llvm import *
from llvm.core import *

from mytokens import *

start = 'program'

precedence = (
    ('left', 'PLUS', 'MINUS'),
    ('left', 'MULTIPLY', 'DIVIDE'),
)

varlist_types = []
varlist_decls = []
params = {}
tmps = {}

def p_program(p):
    'program : decl stmtlist'
    global builder
    # IMPLEMENT: correct return expression
    builder.ret(ConstantInt.int(ty_int,0))
    p[0] = None

def p_decl_vars(p):
    'decl : VARS varlist SEMI'
    global params
    global builder
    global outFile

    name = outFile.split('.')[0]

    # Build function type
    funType = Type.function(ty_int, varlist_types)

    # Build function and add to module
    function = my_module.add_function(funType,name)

    # Implement: map varlist to arguments somehow
    
    # Add BasicBlock to function
    basicBlock = function.append_basic_block("entry")

    # IRBuilder
    builder = Builder.new(basicBlock)

def p_varlist(p):
    '''varlist : varlist COMMA ID
               | ID '''
    global varlist_decls
    varlist_types.append(ty_int)

def p_stmtlist(p):
    '''stmtlist : stmt
                | stmtlist stmt'''
    if len(p)==2:
        p[0] = p[1]
    else:
        p[0] = p[2]

def p_stmt(p):
    'stmt : TMP ASSIGN expression SEMI'    
    # Implement: remember that TMP is associated with expression

def p_expression_plus(p):
    'expression : expression PLUS expression'
    global builder
    # Implement: addition

def p_expression_minus(p):
    'expression : expression MINUS expression'
    global builder
    # Implement subtraction

def p_expression_negation(p):
    'expression : MINUS expression'
    global builder
    # Implement negation

def p_expression_multiply(p):
    'expression : expression MULTIPLY expression'
    global builder
    #implement multiplication

def p_expression_div(p):
    'expression : expression DIVIDE expression'
    global builder
    #Implement division

def p_expression_num(p):
    'expression : NUM'
    #implement build constant

def p_expression_tmp(p):
    'expression : TMP'
    #implement: get expression assigned to TMP

def p_expression_id(p):
    'expression : ID'
    #implement: get expression assigned to ID

# Error rule for syntax errors
def p_error(p):
    print "Syntax error in input!"

# Build the lexer and try it out

lexer = lex.lex()
# Build the parser
parser = yacc.yacc()

if len(sys.argv) < 3:
    print "Not enough positional arguments."
    sys.exit(1)

data = open(sys.argv[1],"r").read()

# Create an (empty) module.
my_module = Module.new('my_module')

# All the types involved here are "int"s. This type is represented
# by an object of the llvm.core.Type class:
ty_int = Type.int(bits=64)     

outFile = sys.argv[2]

if len(data)>0:
    # parse the file and build function
   result = parser.parse(data,lexer=lexer)
   # open the output file
   fileout = open(sys.argv[2],"w")
   # save Module to file in bitcode format
   my_module.to_bitcode(fileout)
   


