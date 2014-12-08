#module: mytokens.py

#This module contains scanning rules. No changes are needed.

tokens = (
    'VARS',
    'TMP',
    'ID',
    'NUM',
    'ASSIGN',
    'SEMI',
    'MINUS',
    'PLUS',
    'MULTIPLY',
    'DIVIDE',
    'COMMA'
    )

reserved = {
    'vars' : 'VARS'
}

t_VARS = r'vars'
t_ASSIGN = r'='
t_SEMI = r';'
t_MINUS = r'-'
t_PLUS = r'\+'
t_MULTIPLY = r'\*'
t_DIVIDE = r'\/'
t_COMMA = r','

def t_NUM(t):
    r'\d+'
    t.value = int(t.value)
    return t

def t_ID(t):
    r'[a-z]+'
    t.type = reserved.get(t.value,'ID')    # Check for reserved words
    return t

def t_TMP(t):
    r'\$\d+'
    t.value = t.value[1:]
    return t

# Error handling rule
def t_error(t):
    print "Illegal character '%s'" % t.value[0]
    t.lexer.skip(1)
    
# A string containing ignored characters (spaces and tabs)
t_ignore  = '\n \t'
