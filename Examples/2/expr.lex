%{
#include <stdio.h>
#include <math.h>

#include "llvm-c/Core.h"
#include "llvm-c/BitReader.h"
#include "llvm-c/BitWriter.h"

#include "expr.y.h"

%}

%x COMMENT 


%%

[\n\t ]           ; // ignore whitespace

[Rr][0-9]+        { 
  int regNo = atoi(yytext+1); 
  yylval.reg = regNo; 
  //printf("REG(%s)",yytext);  
  return REG; 
}
[0-9]+           { /*printf("IMM(%s)",yytext);*/  
  int imm = atoi(yytext); 
  yylval.imm = imm; 
  return IMMEDIATE; 
} 

"("               { /*printf("LPAREN\n");*/ return LPAREN; }
")"               { /*printf("RPAREN\n");*/ return RPAREN; }

"["               { /*printf("LBRACKET\n");*/ return LBRACKET; }
"]"               { /*printf("RBRACKET\n");*/ return RBRACKET; }

"+"               { /*printf("PLUS");*/ return PLUS; }
"-"               { /*printf("MINUS");*/ return MINUS; }
"="               { return ASSIGN; }
";"               { return SEMI; }

\/\/.*\n          {; /* printf("Comment");*/ }

"/*"              { BEGIN(COMMENT); }
                
<COMMENT>{    // These rules only apply during the COMMENT state
"*/"          BEGIN(0);  // return to normal state at end of comment
.             ;  // ignore all characters
\n            ; // ingore newlines
}

%%
