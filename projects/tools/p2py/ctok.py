#module: mytokens.py

#This module contains scanning rules. No changes are needed.

tokens = (
    'SEMICOLON',
    'COLON',
    'COMMA',
    'LBRACE',
    'RBRACE',
    'LPAREN',
    'RPAREN',
    'PLUS',
    'MINUS',
    'STAR',
    'DIV',
    'MOD',
    'LTE',
    'GTE',
    'LT',
    'GT',
    'EQ',
    'NEQ',
    'NOT',
    'ASSIGN',
    'AMPERSAND',
    'LOGICAL_AND',
    'LOGICAL_OR',
    'BITWISE_OR',
    'BITWISE_XOR',
    'LSHIFT',
    'RSHIFT',
    'BITWISE_INVERT',
    'QUESTION_MARK',
    'INT',
    'VOID',
    'FOR',
    'WHILE',
    'IF',
    'ELSE',
    'RETURN',
    'BREAK',
    'CONTINUE',
    'NUMBER',
    'ID',
    )

reserved = {
    'for'      : 'FOR',
    'while'    : 'WHILE',
    'break'    : 'BREAK',
    'continue' : 'CONTINUE',
    'int'      : 'INT',
    'void'     : 'VOID',
    'if'       : 'IF',
    'else'     : 'ELSE',
    'return'   : 'RETURN'
}

t_SEMICOLON = r';'
t_COLON = r':'
t_COMMA = r','
t_LBRACE = r'\{'
t_RBRACE = r'\}'
t_LPAREN = r'\('
t_RPAREN = r'\)'
t_MINUS = r'-'
t_PLUS = r'\+'
t_STAR = r'\*'
t_DIV = r'\/'
t_LTE = r'<='
t_GTE = r'>='
t_LT = r'<'
t_GT = r'>'
t_EQ = r'=='
t_NEQ = r'!='
t_NOT = r'!'
t_ASSIGN = r'='
t_AMPERSAND = r'&'
t_LOGICAL_AND = r'&&'
t_LOGICAL_OR = r'\|\|'
t_BITWISE_OR = r'\|'
t_BITWISE_XOR = r'\^'
t_LSHIFT = r'<<'
t_RSHIFT = r'>>'
t_BITWISE_INVERT = r'~'
t_QUESTION_MARK = r'\?'

def t_NUMBER(t):
    r'\d+'
    t.value = int(t.value)
    return t

def t_ID(t):
    r'[a-zA-Z_][a-zA-Z_0-9]*'
    t.type = reserved.get(t.value,'ID')    # Check for reserved words
    return t

def t_COMMENT(t):
    r'(/\*(.|\n)*\*/)|(//.*)'
    pass

# Error handling rule
def t_error(t):
    print "Illegal character '%s'" % t.value[0]
    t.lexer.skip(1)
    
# A string containing ignored characters (spaces and tabs)
t_ignore  = '\n \t'
