
%{
#include <stdio.h>

FILE *yyin;
%}

%union {
  int val;
}

%token NUMBER PLUS MINUS LPAREN RPAREN NEWLINE 

%type <val> NUMBER expr

%left MINUS PLUS

%start expr_list

%%

expr_list:   
              expr NEWLINE            { printf("=%d\n",$1); }
            | expr_list expr NEWLINE  { printf("=%d\n",$2); }
;

expr :   expr PLUS expr                  { $$ = $1 + $3; }
       | expr MINUS expr                 { $$ = $1 - $3; } 
       | LPAREN expr RPAREN              { $$ = $2; }
        | NUMBER                          { $$ = $1 ; }
       | MINUS expr                      { $$ = - $2 ; }
;

%%

int yyerror(char *msg) {
  printf("%s",msg);
  return 0;
}

int main()
{
  yyin = stdin;

  yyparse();

  return 0;
}
