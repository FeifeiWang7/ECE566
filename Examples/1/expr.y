%{
#include <stdio.h>

void yyerror(const char *);

int g=0;
 int regCnt=8;
int myprototype();

 typedef struct regOrInt {
   int is_reg; // 0 means imm, and 1 means reg
   int reg;
   int imm;
 } RegOrInt;

%}

%token IMMEDIATE REG PLUS MINUS LPAREN RPAREN LBRACKET RBRACKET ASSIGN SEMI
%start program
%left MINUS PLUS

%union {
  RegOrInt ee;
  int reg;
  int imm;
}

%type <reg> REG
%type <imm> IMMEDIATE
%type <ee> expr

%%

program : REG ASSIGN expr SEMI  { 
  struct regOrInt e = $3;
  if (e.is_reg)
    printf("\nADD R%d,R%d,0",$1,e.reg);
  else {
    printf("\nAND R%d,R%d,0",$1,$1);
    printf("\nADD R%d,R%d,%d",$1,$1,e.imm); 
  }
  return 0; 
}
;

expr :   IMMEDIATE  
{
  struct regOrInt e;
  if ($1 < 16 && $1 >= -16) {
    // do something different
    e.is_reg=0;
    e.imm = $1;
    $$ = e;
  }
  else {
    int r=regCnt++;
    printf("\nAND R%d,R%d,0",r,r);
    printf("\nADD R%d,R%d,%d",r,r,$1);
    e.is_reg=1;
    e.reg = r;
    $$ = e;
  }
}
| REG
{
  struct regOrInt e;
  e.is_reg=1;
  e.reg = $1;
  //  int r=regCnt++;
  //printf("\nADD R%d,R%d,0",r,$1); 
  $$ = e;
}
| expr PLUS expr 
{
  struct regOrInt e1 = $1;
  struct regOrInt e3 = $3;

  int r=regCnt++;
  if (e1.is_reg && e3.is_reg)
    printf("\nADD R%d,R%d,R%d",r,e1.reg,e3.reg);
  else if (!e1.is_reg && !e3.is_reg) {
    printf("\nAND R%d,R%d,0",r,r);
    printf("\nADD R%d,R%d,%d",r,r,e1.imm);
    printf("\nADD R%d,R%d,%d",r,r,e3.imm);
  } else {
    // one reg and one imm

    if (e1.is_reg) { 
      printf("\nADD R%d,R%d,%d",r,e1.reg,e3.imm);
    } else {
      printf("\nADD R%d,R%d,%d",r,e3.reg,e1.imm);
    }
  }
  struct regOrInt out;
  out.is_reg=1;
  out.reg = r;
    
  $$ = out;
}
| expr MINUS expr
{
  struct regOrInt out;
  int r=regCnt++;
  //printf("\nSUB R%d,R%d,R%d",r,$1,$3);
  
  out.is_reg=1;
  out.reg=r;

  $$ = out;
}
| LPAREN expr RPAREN
{
  $$ = $2;
}
| MINUS expr
{
  struct regOrInt out;
  int r=regCnt++;
  //printf("\nSUB R%d,R%d,R%d",r,$1,$3);
  
  out.is_reg=1;
  out.reg=r;

  $$ = out;
}
| LBRACKET expr RBRACKET
{
  struct regOrInt out;
  int r=regCnt++;
  
  if ($2.is_reg)
    printf("\nLDR R%d,R%d,0",r,$2.reg);
  else
    {
      int r2=regCnt++;
      printf("\nAND R%d,R%d,0",r2,r2);
      printf("\nADD R%d,R%d,%d",r2,r2,$2.imm);
      printf("\nLDR R%d,R%d,0",r,r2);
    }

  out.is_reg=1;
  out.reg=r;
  $$ = out; 
}
;


%%

int myprototype() {
  printf("This is an awesome function!\n");
  return 0;
}

void yyerror(const char *msg){
  printf("%s",msg);
}

int main() {
  yyparse();
  printf("\n");
  return 0;
}
