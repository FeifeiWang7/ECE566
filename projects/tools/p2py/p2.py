#!/usr/bin/env python2

import sys
import ply.lex as lex
import ply.yacc as yacc

from optparse import OptionParser

# Import the llvmpy modules.
from llvm import *
from llvm.core import *

from ctok import *

start = 'translation_unit'

precedence = (
    ('nonassoc', 'ELSE'),    
)

def p_translation_unit(p):
    ''' translation_unit : external_declaration 
                         | translation_unit external_declaration '''
    # do nothing

def p_external_decl(p):
    ''' external_declaration : function_definition 
                             | declaration '''


def p_function_definition(p):
    ''' function_definition : type_specifier ID LPAREN param_list_opt RPAREN fundef_pushscope1 compound_stmt 
                           | type_specifier STAR ID LPAREN param_list_opt RPAREN fundef_pushscope2 compound_stmt '''
    
def p_fundef_pushscope1(p):
    'fundef_pushscope1 :'
    BuildFunction(p[-5],p[-4],p[-2])

def p_fundef_pushscope2(p):
    'fundef_pushscope2 :'
    BuildFunction(Type.pointer(p[-6],0),p[-4],p[-2])

def BuildFunction(rtype,name,params):
    global builder
    global localMap
    global function
    vlist = []
    for p in params[:]:
        vlist.append(p[0])        
        
    funType = Type.function(rtype,vlist)
    function = my_module.add_function(funType,name)

    basicBlock = function.append_basic_block("entry")
    builder = Builder.new(basicBlock)

    i=0
    for p in params[:]:
        a = builder.alloca(p[0],p[1])
        # establish a map from declarator name to alloca
        # use this address to load/store from declarator
        localMap[p[1]] = a
        builder.store(function.args[i],a,False)
        i = i+1

def p_declaration(p):
    '''declaration : type_specifier STAR declarator SEMICOLON
                   | type_specifier declarator SEMICOLON '''

def p_declaration_list(p):
    '''declaration_list : declaration 
                        | declaration_list declaration '''

def p_type_specifier(p):
    'type_specifier : INT'
    p[0] = ty_int

def p_type_specifier_void(p):
    'type_specifier : VOID'
    p[0] = ty_void

def p_declarator(p):
    ' declarator : ID '
    p[0] = p[1]

def p_empty(p):
    'empty :'
    p[0] = None
    pass

def p_param_list_opt(p):
    'param_list_opt : param_list'
    p[0] = p[1]

def p_param_list_opt_empty(p):
    'param_list_opt : empty'
    p[0] = []

def p_param_list_append(p):
    ''' param_list : param_list COMMA type_specifier declarator
                   | param_list COMMA type_specifier STAR declarator
                   | param_list COMMA type_specifier '''
    if len(p)==6:
        p[0] = p[1] + [ (Type.pointer(p[3],0),p[5]) ]
    elif len(p)==5:
        p[0] = p[1] + [ (p[3],p[4]) ]
    else:
        p[0] = p[1] + [(p[3],None)]


def p_param_list(p):
    ''' param_list : type_specifier declarator
                   | type_specifier STAR declarator
                   | type_specifier '''
    if len(p)==4:
        p[0] = [ (Type.pointer(p[1],0),p[3]) ]
    elif len(p)==3:
        p[0] = [ (p[1],p[2]) ]
    else:
        p[0] = [ (p[1],None) ]

def p_statement(p):
    ''' statement : compound_stmt 
                  | for_stmt 
                  | while_stmt 
                  | expr_stmt 
                  | if_else_stmt 
                  | break_stmt 
                  | continue_stmt 
                  | return_stmt '''
    

def p_compound_stmt(p):
    ' compound_stmt : LBRACE declaration_list_opt statement_list_opt RBRACE '

def p_declaration_list_opt(p):
    ''' declaration_list_opt : empty 
                           | declaration_list '''

def p_statement_list_opt(p):
    ''' statement_list_opt : empty 
                           | statement_list '''

def p_statement_list(p):
    ''' statement_list : statement 
                       | statement_list statement '''

def p_for_stmt(p):
    ''' for_stmt :  FOR LPAREN expr_opt SEMICOLON expr_opt SEMICOLON expr_opt RPAREN statement
                    '''

def p_while_stmt(p):
    ' while_stmt : WHILE LPAREN expression RPAREN statement '

def p_expr_stmt(p):
    ''' expr_stmt : SEMICOLON 
                | expression SEMICOLON '''

def p_if_else_stmt(p):
    ' if_else_stmt : IF LPAREN expression RPAREN statement ELSE statement '

def p_break_stmt(p):
    ' break_stmt : BREAK SEMICOLON '

def p_continue_stmt(p):
    ' continue_stmt : CONTINUE SEMICOLON '

def p_return_stmt(p):
    ''' return_stmt : RETURN SEMICOLON 
                    | RETURN expression SEMICOLON '''
    global builder
    builder.ret(ConstantInt.int(ty_int,0))

def p_expr_opt(p):
    ''' expr_opt : empty 
               | expression '''

def p_expression(p):
    ' expression : assignment_expression '

def p_assignment_expression(p):
    ''' assignment_expression : conditional_expression 
                              | lhs_expression ASSIGN assignment_expression '''

def p_conditional_expression(p):
    ''' conditional_expression : logical_OR_expression
                               | logical_OR_expression QUESTION_MARK expression COLON conditional_expression '''

def p_logical_OR_expression(p):
    ''' logical_OR_expression : logical_AND_expression
                              | logical_OR_expression LOGICAL_OR logical_AND_expression '''

def p_logical_AND_expression(p):
    ''' logical_AND_expression : inclusive_OR_expression
                               | logical_AND_expression LOGICAL_AND inclusive_OR_expression '''

def p_inclusive_OR_expression(p):
    ''' inclusive_OR_expression : exclusive_OR_expression
                                | inclusive_OR_expression BITWISE_OR exclusive_OR_expression '''

def p_exclusive_OR_expression(p):
    ''' exclusive_OR_expression : AND_expression
                                | exclusive_OR_expression BITWISE_XOR AND_expression '''

def p_AND_expression(p):
    ''' AND_expression : equality_expression 
                       | AND_expression AMPERSAND equality_expression '''

def p_equality_expression(p):
    ''' equality_expression : relational_expression
                            | equality_expression EQ relational_expression
                            | equality_expression NEQ relational_expression '''
 
def p_relational_expression(p):
    ''' relational_expression : shift_expression
                              | relational_expression LT shift_expression
                              | relational_expression GT shift_expression
                              | relational_expression LTE shift_expression
                              | relational_expression GTE shift_expression '''

def p_shift_expression(p):
    ''' shift_expression : additive_expression
                         | shift_expression LSHIFT additive_expression
                         | shift_expression RSHIFT additive_expression '''

def p_additive_expression(p):
    ''' additive_expression : multiplicative_expression
                            | additive_expression PLUS multiplicative_expression
                            | additive_expression MINUS multiplicative_expression '''

def p_multiplicative_expression(p):
    ''' multiplicative_expression : unary_expression
                                  | multiplicative_expression STAR unary_expression
                                  | multiplicative_expression DIV unary_expression
                                  | multiplicative_expression MOD unary_expression '''

def p_lhs_expression(p):
    ''' lhs_expression : ID 
                       | STAR ID '''

def p_unary_expression(p):
    ''' unary_expression : primary_expression
                         | AMPERSAND primary_expression
                         | STAR primary_expression
                         | MINUS unary_expression
                         | BITWISE_INVERT unary_expression
                         | NOT unary_expression '''

def p_primary_expression(p):
    ''' primary_expression : ID 
                           | constant 
                           | LPAREN expression RPAREN '''

def p_constant(p):
    ' constant : NUMBER '
    p[0] = p[1]


# Error rule for syntax errors
def p_error(p):
    print "Syntax error in input!",p.type,p.value

# Build the lexer and try it out

lexer = lex.lex()
# Build the parser
parser = yacc.yacc()

opts = OptionParser()

opts.add_option("-v","--verbose",action="store_true",dest="verbose",default=False,
                  help="print copious output to screen")
opts.add_option("-o","--o",action="store",dest="output",default="out.bc",
                  help="set name of output file [default=out.bc]")

(options,args) = opts.parse_args(sys.argv[1:])

if len(args)==0:
    print "Not enough positional arguments."
    sys.exit(1)

# Create an (empty) module.
my_module = Module.new(args[0])

#contains a map from local declarator name to alloca
localMap = {}

# All the types involved here are "int"s. This type is represented
# by an object of the llvm.core.Type class:
ty_int = Type.int(bits=32)     
ty_void = Type.void()

for f in args[:]:
    data = open(f,"r").read()
    if len(data)>0:
        # parse the file and build function
        result = parser.parse(data,lexer=lexer)

# save Module to file in bitcode format
fileout = open(options.output,"w")
my_module.to_bitcode(fileout)
   


