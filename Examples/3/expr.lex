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

"if" {
  return IF;
}

"while" {
  return WHILE;
}

"return" {
  return RETURN;
}

[a-zA-Z]        { 
  yylval.id = strdup(yytext);
  return ID; 
}

[0-9]+           { 
  int imm = atoi(yytext); 
  yylval.imm = imm; 
  return IMMEDIATE; 
} 

"("               { return LPAREN; }
")"               { return RPAREN; }

"{"               { return LBRACE; }
"}"               { return RBRACE; }


"+"               { return PLUS; }
"-"               { return MINUS; }
"/"               { return DIVIDE; }
"*"               { return MULTIPLY; }

"!"               { return NOT; }


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
