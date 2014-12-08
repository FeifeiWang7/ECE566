/* modified by fwang12, Feifei Wang*/
%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Type.h"

#include "llvm/Bitcode/ReaderWriter.h"
#include "llvm/ADT/StringSet.h"
#include "llvm/ADT/StringMap.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Support/SystemUtils.h"
#include "llvm/Support/ToolOutputFile.h"
#include "llvm/IR/IRBuilder.h"

#include <memory>
#include <algorithm>

#include <errno.h>
#include <search.h>

using namespace llvm;

extern std::string OutputFilename;
extern Module      *M;
extern int correct;
Function    *Fun;
BasicBlock  *BB;
IRBuilder<> *Builder;
 
std::vector<const char*> decls; 

StringMap<Value*> params; 
StringMap<Value*> tmps; 

int params_cnt=0;

void yyerror(const char*);

int yylex();

%}

%union {
  char *tmp;
  int num;
  char *id;
  Value* val;
}

%token ASSIGN SEMI COMMA MINUS PLUS VARS
%token <tmp> TMP 
%token <num> NUM 
%token <id> IDENT
%type <val> expr stmt stmtlist;

%left PLUS MINUS
%left MULTIPLY DIVIDE
%left CARET
%start program

%%

program: decl stmtlist 
{ 
  Builder->CreateRet($2); //return 
  delete Builder;
	correct = 1;
}
  ;

decl: VARS varlist SEMI 
{  
  IntegerType* Integer = IntegerType::get(getGlobalContext(),64);

  std::vector<Type*> v;
  int i;
  for(i=0; i<params_cnt; i++)
    v.push_back(Integer);

  ArrayRef<Type*> Params(v);

  FunctionType* FunType = FunctionType::get(Integer,Params,false);

  std::string fname = OutputFilename;
  int index = fname.find('.');
  if (index!=std::string::npos)
    fname[index] = 0;
  
  Fun = Function::Create(FunType,GlobalValue::ExternalLinkage,
			 fname.c_str(),M);
  Twine T("entry");

  Function::ArgumentListType &list = Fun->getArgumentList();
  Function::ArgumentListType::iterator it;
  int k=0;
  for(it=list.begin(); it!=list.end(); it++,k++)
    {
      if ( !(*it).hasName() ) {
	(*it).setName(decls.at(k));
      }
    }


  BB = BasicBlock::Create(getGlobalContext(),T,Fun);

  Builder = new IRBuilder<>(getGlobalContext());
  Builder->SetInsertPoint(BB);

  Function::arg_iterator I = Fun->arg_begin(); //mapping decls and params
  for(i=0; I!=Fun->arg_end();i++, I++)
	params.GetOrCreateValue(decls.at(i),I);
}
;

varlist:   varlist COMMA IDENT
{
	for(std::vector<const char*>::iterator it = decls.begin(); it != decls.end(); ++it)
	{
		if(strcmp(*it, $3)== 0) //if var already exists
		{
			yyerror("In Vars multiple definition\n");
			YYERROR;
		}
	}
	decls.push_back($3);
  params_cnt++;
}
| IDENT
{
	decls.push_back($1);
  params_cnt++;
}
;

stmtlist:  stmtlist stmt { $$ = $2; }
| stmt                   { $$ = $1; }
;         

stmt: TMP ASSIGN expr SEMI
{
	tmps[$1] = $3; //add to map
	$$ = tmps.lookup($1); //give value to $$
}

expr:   expr MINUS expr
{
	$$ = Builder -> CreateSub($1, $3, "", false, false);
} 
     | expr PLUS expr
{
	$$ = Builder -> CreateAdd($1, $3, "", false, false);
}
      | MINUS expr 
{
	$$ = Builder -> CreateNeg($2, "", false, false);
}
      | expr MULTIPLY expr
{
	$$ = Builder -> CreateMul($1, $3, "", false, false);
}
      | expr DIVIDE expr
{
	$$ = Builder -> CreateSDiv($1, $3, "", false);
}
      | expr CARET expr
{
	if(LLVMIsAConstantInt(wrap($3))) //expr2 must be constant
	{
		long i = 0, count = LLVMConstIntGetSExtValue(wrap($3));
		if(count < 0)
		{
			$$ = unwrap(LLVMConstInt(LLVMInt64Type(), 1, 0)); 
			for( i = count; i < 0; i ++)
				$$ = Builder -> CreateSDiv($$, $1, "", false);
		}
		else if(count == 0) $$ = unwrap(LLVMConstInt(LLVMInt64Type(), 1,0));
		else{
			$$ = unwrap(LLVMConstInt(LLVMInt64Type(), 1, 0)); 
			for(i = 0; i < count; i ++)
				$$ = Builder -> CreateMul($$, $1, "", false, false);
		}
	}
	else
	{
		yyerror("The second param of CARET is not constant!"); 
		YYERROR;
	}
}
      | NUM
{
	$$ = unwrap(LLVMConstInt(LLVMInt64Type(), $1, 0)); 
}
      | IDENT
{
	$$ = params.lookup($1);
	if($$ == NULL) //if id doesn't exist
	{
		yyerror("No param named this!"); 
		YYERROR;
	}
}
      | TMP
{
	$$ = tmps.lookup($1);
	if($$ == NULL) //if reg doesn't exist
	{
		yyerror("No tmp named this!"); 
		YYERROR;
	}
		
}
;

%%

void initialize()
{
}

void yyerror(const char *msg)
{
	correct = 0;
  printf("ERROR: %s\n",msg);
}
