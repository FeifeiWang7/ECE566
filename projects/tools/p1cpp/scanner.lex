/* modified by fwang12, Feifei Wang*/
%{ 
/* P1. Implements scanner.  No additional changes required. */
#include <string.h>
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Type.h"

using namespace llvm;

#include "parser.hpp" 
extern int correct;
%}
 
%% 

[\t \n]+        ;

vars            { return VARS; }
\$[a-z0-9]+	{ yylval.tmp = strdup(yytext); return TMP; } 
[a-zA-Z_]+          { yylval.id = strdup(yytext); return IDENT; } 

[0-9]+          { yylval.num = atoi(yytext); return NUM; }

"="	{ return ASSIGN; } 
";"	{ return SEMI; } 
"-"	{ return MINUS; } 
"+"	{ return PLUS; } 
"*"	{ return MULTIPLY; } 
"/"	{ return DIVIDE; } 
","     { return COMMA; }
"^"	{ return CARET; }
.	{ correct = 0; printf("ERROR: Input is not valid!\n"); exit(1);}

%%
