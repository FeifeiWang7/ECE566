%{
#include <string.h>
#include <unistd.h>

#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Type.h"

#include <memory>
#include <algorithm>

#include "list.h"

#include "minic.y.hpp"


#include <errno.h>
#include <search.h>


using namespace llvm;


int line_num=1;

void lexical_error(const char *);
%}

%x comment

DIGIT   [0-9]
ID      [a-zA-Z_][a-zA-Z_0-9]*
SPACE   [\t ]*
NEWLINE [\n\r]

%{
  /* additional code here */
%}

%option nounput

%%

{SPACE}                   ;
{NEWLINE}                 ++line_num;

";"                     return SEMICOLON; //
":"                     return COLON; //
","                     return COMMA; //

"{"                     return LBRACE; //
"}"                     return RBRACE; //
"("                     return LPAREN;
")"                     return RPAREN;
"["                     return LBRACKET;
"]"                     return RBRACKET;
"+"                     return PLUS;
"-"                     return MINUS;
"*"                     return STAR;
"/"                     return DIV;
"%"                     return MOD;
"<="                    return LE;
">="                    return GE;
"<"                     return LT;
">"                     return GT;
"=="                    return EQ;
"!="                    return NEQ;
"!"                     return NOT;
"="                     return ASSIGN; //
"->"                    return ARROW;
"."                     return DOT;
"&"                     return BITWISE_AND;
"&&"                    return LOGICAL_AND;
"||"                    return LOGICAL_OR;
"|"                     return BITWISE_OR;
"^"                     return BITWISE_XOR;
"<<"                    return LSHIFT;
">>"                    return RSHIFT;
"~"                     return BITWISE_INVERT;
"?"                     return QUESTION_MARK; //

int               return INT;
void              return VOID;
for               return FOR;
while             return WHILE;
if                return IF;
else              return ELSE;
do                return DO;
struct            return STRUCT;
sizeof            return SIZEOF;
return            return RETURN;

switch            lexical_error("No switch statement in MiniC!");                       
break             return BREAK;
continue          return CONTINUE;


{DIGIT}*         { yylval.num = atoi(yytext); return NUMBER; }
{ID}              { yylval.id = strdup(yytext); return IDENT; }

"/*"                    { BEGIN(comment); }

<comment>[^*\n]*        /* eat anything that's not a '*' */
<comment>"*"+[^*/\n]*   /* eat up '*'s not followed by '/'s */
<comment>{NEWLINE}      ++line_num;
<comment>"*"+"/"        BEGIN(0);

.                       lexical_error("Unmatched character");      

%%

void lexical_error(const char *msg)
{
  printf("Lexical error(%d): %s\n", line_num, msg);
  exit(-1);
}
