%{
#include <stdio.h>

#include "llvm-c/Core.h"
#include "llvm-c/BitReader.h"
#include "llvm-c/BitWriter.h"

int yylex();

void yyerror(const char *);

int g=0;
 int regCnt=8;
int myprototype();

LLVMBuilderRef Builder;

LLVMValueRef regs[8] = {NULL};

%}

%token IMMEDIATE REG PLUS MINUS LPAREN RPAREN LBRACKET RBRACKET ASSIGN SEMI
%start program
%left MINUS PLUS
%left MULTIPLY

%union {
  int reg;
  int imm;
  LLVMValueRef val;
}

%type <reg> REG
%type <imm> IMMEDIATE
%type <val> expr

%%

program : REG ASSIGN expr SEMI  { 
  regs[$1] = $3;
 }
| program REG ASSIGN expr SEMI {
  regs[$2] = $4;
}
| program SEMI {
  return 0; // end of the program
}

;

expr :   IMMEDIATE  
{
  $$ = LLVMConstInt(LLVMInt64Type(), $1, 0);
}
| REG
{
  // referring to a register
  // Should do some error checking
  if (regs[$1]) 
    $$ = regs[$1];
  else {
    printf("Used a register without giving it a value!\n");
    return -1;
  }
}
| expr PLUS expr 
{
  $$ = LLVMBuildAdd(Builder, $1, $3, "");
}
| expr MINUS expr
{
  $$ = LLVMBuildSub(Builder, $1, $3, "");
}
| LPAREN expr RPAREN
{
  $$ = $2;
}
| MINUS expr
{
  $$ = LLVMBuildNeg(Builder, $2, "");
}
| LBRACKET expr RBRACKET
{
  // creating a load
  LLVMTypeRef int64ptr = LLVMPointerType(LLVMInt64Type(),0);
  LLVMValueRef int2ptr = LLVMBuildIntToPtr(Builder,$2,int64ptr,"");
  $$ = LLVMBuildLoad(Builder, int2ptr, "");
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
  
  //http://llvm.org/doxygen/group__LLVMCCoreModule.html
  LLVMModuleRef Module = LLVMModuleCreateWithName("main");
  
  // Make a function that's the instructions
  LLVMTypeRef VoidTy = LLVMVoidType();
  LLVMTypeRef VoidFnTy = LLVMFunctionType(VoidTy, NULL, 0, 0);

  LLVMValueRef Fn = LLVMAddFunction(Module,"main",VoidFnTy);
  //Now, we have a function inside our module

  //And, a basic block:
  LLVMBasicBlockRef BB = LLVMAppendBasicBlock(Fn,"entry");

  // Create Builder object that make instructions for us
  Builder = LLVMCreateBuilder();
  LLVMPositionBuilderAtEnd(Builder,BB);
  
  if (yyparse()==0) {
    // all is good
    // dump LLVM IR to file
    LLVMBuildRetVoid(Builder);
    LLVMWriteBitcodeToFile(Module,"main.bc");
  } else  {
    printf("There was a problem!  See errors above.\n");
  }
  return 0;
}
