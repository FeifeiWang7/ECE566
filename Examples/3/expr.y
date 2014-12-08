%{
#include <stdio.h>

#include "llvm-c/Core.h"
#include "llvm-c/BitReader.h"
#include "llvm-c/BitWriter.h"

#include "uthash.h"

int yylex();

void yyerror(const char *);

LLVMValueRef Fn;
LLVMBuilderRef Builder;
LLVMBasicBlockRef BBjoin;

struct IDMap{
  char *key;                  /* key */
  LLVMValueRef val;                /* data */
  UT_hash_handle hh;         /* makes this structure hashable */
}; 

struct IDMap *map = NULL;    /* important! initialize to NULL */

void addID(char *id, LLVMValueRef val) { 
  struct IDMap *s; 
  s = (struct IDMap*)malloc(sizeof(struct IDMap)); 
  s->key = strdup(id); 
  s->val = val; 
  HASH_ADD_KEYPTR( hh, map, s->key, strlen(s->key), s ); 
}

void createOrUpdateID(char *id, LLVMValueRef val) { 
  struct IDMap *s;
  HASH_FIND_STR( map, id, s );  /* s: output pointer */
  if (s) 
    {
      s->val = val;
    }
  else {
    s = (struct IDMap*)malloc(sizeof(struct IDMap)); 
    s->key = strdup(id); 
    s->val = val; 
    HASH_ADD_KEYPTR( hh, map, s->key, strlen(s->key), s ); 
  }
}

LLVMValueRef getValueForID(char *id) {
  struct IDMap *s;
  HASH_FIND_STR( map, id, s );  /* s: output pointer */
  if (s) 
    return s->val;
  else 
    return NULL; // returns NULL if not found
}

%}

%token IMMEDIATE ID PLUS MINUS LPAREN RPAREN ASSIGN SEMI MULTIPLY DIVIDE IF
%token WHILE LBRACE RBRACE RETURN NOT
%start program

%left MINUS PLUS
%left MULTIPLY DIVIDE
%right NOT

%union {
  char *id;
  int imm;
  LLVMValueRef val;
  LLVMBasicBlockRef bb;
}

%type <id> ID 
%type <imm> IMMEDIATE
%type <val> expr

%%

program : LBRACE stmtlist RETURN expr SEMI RBRACE
{
  LLVMBuildRet(Builder,$4);
  YYACCEPT;
}
;

stmtlist :    stmt
           |  stmtlist stmt
;

stmt:   ID ASSIGN expr SEMI {
  LLVMValueRef var = getValueForID($1);
  if (var==NULL) {
    var = LLVMBuildAlloca(Builder,LLVMInt64Type(),"");
    createOrUpdateID($1,var);
  }
  LLVMBuildStore(Builder,$3,var);
 }

| IF expr {
  LLVMBasicBlockRef then = LLVMAppendBasicBlock(Fn,"then.block");
  LLVMBasicBlockRef join = LLVMAppendBasicBlock(Fn,"join.block");

  LLVMValueRef zero = LLVMConstInt(LLVMTypeOf($2),0,1); 

  LLVMValueRef cond = LLVMBuildICmp(Builder, LLVMIntNE, $2,
                                  zero,"cond");

  LLVMValueRef br = LLVMBuildCondBr(Builder,cond,then,join);
  // can't put anything else in this basic block, must reposition builder

  LLVMPositionBuilderAtEnd(Builder,then);
  $<bb>$ = join;
 }
LBRACE stmtlist RBRACE
{
  LLVMBasicBlockRef join = $<bb>3;
  LLVMBuildBr(Builder,join); // from then -> join
  LLVMPositionBuilderAtEnd(Builder,join);
}        

| WHILE 
{
  LLVMBasicBlockRef cond = LLVMAppendBasicBlock(Fn,"while.cond");
  LLVMBuildBr(Builder,cond);
  LLVMPositionBuilderAtEnd(Builder, cond);
  $<bb>$ = cond;

} expr {

  LLVMBasicBlockRef body = LLVMAppendBasicBlock(Fn,"while.body");
  LLVMBasicBlockRef join = LLVMAppendBasicBlock(Fn,"while.join");

  LLVMValueRef zero = LLVMConstInt(LLVMTypeOf($3),0,1); 
  LLVMValueRef cond = LLVMBuildICmp(Builder, LLVMIntNE, $3,
                                  zero,"cond");

  LLVMValueRef br = LLVMBuildCondBr(Builder,cond,body,join);
  // can't put anything else in this basic block, must reposition builder

  LLVMPositionBuilderAtEnd(Builder,body);
  $<bb>$ = join;

}
LBRACE stmtlist RBRACE
{
  // Gets the parent of the instruction => a basic block
  LLVMBasicBlockRef cond = $<bb>2;
  LLVMBuildBr(Builder,cond);
  LLVMPositionBuilderAtEnd(Builder, $<bb>4);			   
}

| SEMI
;

expr :   IMMEDIATE  
{
  $$ = LLVMConstInt(LLVMInt64Type(), $1, 0);
}
| ID
{
  // First, get address
  LLVMValueRef alloca = getValueForID($1);
  
  // Load value
  $$ = LLVMBuildLoad(Builder,alloca,$1);
  
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
| expr DIVIDE expr
{
  $$ = LLVMBuildSDiv(Builder, $1, $3, "");
}
| expr MULTIPLY expr
{
  $$ = LLVMBuildMul(Builder,$1,$3,"");
}
| NOT expr
{
  // fill this in
}
;


%%

void yyerror(const char *msg){
  printf("%s",msg);
}

int main(int argc, char **argv) {
  
  const char *filename = "main.bc";

  if (argc>1) {
    filename = argv[1];
  }

  //http://llvm.org/doxygen/group__LLVMCCoreModule.html
  LLVMModuleRef Module = LLVMModuleCreateWithName("main");
  
  // Make a function that holds the instructions
  LLVMTypeRef VoidTy = LLVMVoidType();
  LLVMTypeRef IntFnTy = LLVMFunctionType(LLVMInt64Type(), NULL, 0, 0);

  Fn = LLVMAddFunction(Module,"myfunction",IntFnTy);
  //Now, we have a function inside our module

  //And, a basic block:
  LLVMBasicBlockRef BB = LLVMAppendBasicBlock(Fn,"entry");

  // Create Builder object that make instructions for us
  Builder = LLVMCreateBuilder();
  LLVMPositionBuilderAtEnd(Builder,BB);
  
  if (yyparse()==0) {
    // all is good
    LLVMWriteBitcodeToFile(Module,filename);
  } else  {
    printf("There was a problem!  See errors above.\n");
  }
  return 0;
}
