/* modified by Feifei Wang, fwang12 */
%{
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/GlobalVariable.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Bitcode/ReaderWriter.h"
#include "llvm/ADT/StringSet.h"
#include "llvm/ADT/StringMap.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Support/SystemUtils.h"
#include "llvm/Support/ToolOutputFile.h"
#include <memory>
#include <algorithm>
#include "list.h"
#include "symbol.h"
using namespace llvm;
int num_errors;
extern int yylex();  
int yyerror(const char *error);
int parser_error(const char*);
void minic_abort();
char *get_filename();
int get_lineno();
int loops_found=0;
extern Module *M;
Function *Fun;
IRBuilder<> *Builder;
Value* BuildFunction(Type* RetType, const char *name, paramlist_t *params);
%}
%union {
  int num;
  char * id;
  Type*  type;
  Value* value;
  paramlist_t *params;
	BasicBlock *bb;
}
/* same sequence as C language*/
%token SEMICOLON 
%token LBRACE RBRACE 
%left COMMA
%right ASSIGN
%right QUESTION_MARK COLON 
%left LOGICAL_OR
%left LOGICAL_AND
%left BITWISE_OR
%left BITWISE_XOR
%left BITWISE_AND
%left EQ NEQ
%left LT GT LE GE
%left LSHIFT RSHIFT
%left PLUS MINUS
%left STAR DIV MOD
%right UPLUS UMINUS NOT BITWISE_INVERT UAND USTAR /*unary operations*/
%left LPAREN RPAREN LBRACKET RBRACKET DOT ARROW
%token FOR WHILE IF ELSE DO STRUCT SIZEOF RETURN 
%token BREAK CONTINUE
%token INT VOID
%token STATIC AUTO EXTERN TYPEDEF CONST VOLATILE ENUM UNION REGISTER
%token <num> NUMBER
%token <id>  IDENT
%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE
%type <type> type_specifier
%type <id> declarator
%type <params> param_list param_list_opt
%type <value> expr_opt
%type <value> expression
%type <value> assignment_expression
%type <value> conditional_expression
%type <value> logical_OR_expression
%type <value> logical_AND_expression
%type <value> inclusive_OR_expression
%type <value> exclusive_OR_expression
%type <value> AND_expression
%type <value> equality_expression
%type <value> relational_expression
%type <value> shift_expression
%type <value> additive_expression
%type <value> multiplicative_expression
%type <value> cast_expression
%type <value> unary_expression
%type <value> lhs_expression
%type <value> postfix_expression
%type <value> primary_expression
%type <num> constant
%type <tree> break_stmt continue_stmt
%%
translation_unit:	  external_declaration
			| translation_unit external_declaration
;

external_declaration:	  function_definition
{
  if(num_errors>100)
    {
      minic_abort();
    }
  else if(num_errors==0)
    {
      
    }
}
                        | declaration {}
;

function_definition:	  type_specifier IDENT LPAREN param_list_opt RPAREN 
{
  symbol_push_scope();
  BuildFunction($1,$2,$4);  
} 
                          compound_stmt 
{ 
  BasicBlock *BB = Builder->GetInsertBlock();
  if(!BB->getTerminator())
    {
      Builder->CreateRet(ConstantInt::getSigned(
						IntegerType::get(getGlobalContext(),32),0));
    }
  symbol_pop_scope();
}
                        | type_specifier STAR IDENT LPAREN param_list_opt RPAREN 
{
  symbol_push_scope();
  BuildFunction(PointerType::get($1,0),$3,$5);
} 
                          compound_stmt 
{ 
  BasicBlock *BB = Builder->GetInsertBlock();
  if(!BB->getTerminator())
    {
      Builder->CreateRet(ConstantInt::getSigned(
						IntegerType::get(getGlobalContext(),32),0));
    }
  symbol_pop_scope();
}
;

declaration:    type_specifier STAR declarator SEMICOLON
{
  if (is_global_scope())
    {
      Twine name($3);
      new GlobalVariable(*M,(Type*)PointerType::get($1,0),false,GlobalValue::ExternalLinkage,(Constant*)NULL,name);
    } 
  else symbol_insert($3, Builder->CreateAlloca(PointerType::get($1,0),NULL,$3), 0);  

} 
              | type_specifier declarator SEMICOLON
{
  if (is_global_scope())
    {
      Twine name($2);
      new GlobalVariable(*M,(Type*)$1,false,GlobalValue::ExternalLinkage,(Constant*)NULL,name);
    }
  else symbol_insert($2, Builder->CreateAlloca($1,NULL,$2), 0);
} 
;

declaration_list:	   declaration{}
                         | declaration_list declaration  {}
;


type_specifier:		  INT 
{
  $$ = IntegerType::get(getGlobalContext(),32);
}
			| VOID  /* right? */
{
	$$ = Type::getVoidTy(getGlobalContext());	
}
;


declarator:		  IDENT
{
  $$ = $1;
}
;

param_list_opt:           
{ 
  $$ = NULL;
}
                        | param_list
{ 
  $$ = $1;
}
;

param_list:	
			  param_list COMMA type_specifier declarator
{
  $$ = push_param($1,$4,$3);
}
			| param_list COMMA type_specifier STAR declarator
{
  $$ = push_param($1,$5,(Type*)PointerType::get($3,0));
}
                        | param_list COMMA type_specifier
{
  $$ = push_param($1,NULL,$3);
}
			|  type_specifier declarator
{
  $$ = push_param(NULL, $2, $1);
}
			| type_specifier STAR declarator
{
  $$ = push_param(NULL, $3, (Type*)PointerType::get($1,0));
}
                        | type_specifier
{
  $$ = push_param(NULL, NULL, $1);
}
;


statement:		  expr_stmt            
			| compound_stmt        
			| selection_stmt       
			| iteration_stmt       
			| jump_stmt            
                        | break_stmt
                        | continue_stmt
;

expr_stmt:	           SEMICOLON {}
			|  expression SEMICOLON {}
;

compound_stmt:		  LBRACE declaration_list_opt statement_list_opt RBRACE {}
;

declaration_list_opt:	{}
			| declaration_list {}
;

statement_list_opt:	{}
			| statement_list {}
;

statement_list:		statement {}
			| statement_list statement {}
;

break_stmt:               BREAK SEMICOLON
{
	if((get_loop().expr == NULL)&& (get_loop().body == NULL)&& (get_loop().reinit == NULL)&& (get_loop().exit == NULL))
	{
		num_errors++;
		parser_error("Break not in a loop."); //check if in the loop
		exit(1);
	}
	else
	{
		loop_info_t info = get_loop();
		BasicBlock *break_block = BasicBlock::Create(getGlobalContext(),"break.",Fun);
		BasicBlock *break_after = BasicBlock::Create(getGlobalContext(),"break.after",Fun);
		Value* cond  = Builder -> CreateICmpNE(unwrap(LLVMConstInt(wrap(IntegerType::get(getGlobalContext(),32)), 1, 1)), unwrap(LLVMConstInt(wrap(IntegerType::get(getGlobalContext(),32)), 0, 1)),"");
		Builder -> CreateCondBr(cond, break_block, break_after, NULL);
		Builder -> SetInsertPoint(break_block);
		Builder -> CreateBr(info.exit);
		Builder -> SetInsertPoint(break_after);
	}
};

continue_stmt:            CONTINUE SEMICOLON //check if in the loop
{
	if((get_loop().expr == NULL)&& (get_loop().body == NULL)&& (get_loop().reinit == NULL)&& (get_loop().exit == NULL))
	{
		num_errors++;
		parser_error("Continue not in a loop.");
		exit(1);
	}
	else
	{
		loop_info_t info = get_loop();
		BasicBlock *continue_block = BasicBlock::Create(getGlobalContext(),"continue.",Fun);
		BasicBlock *continue_after = BasicBlock::Create(getGlobalContext(),"continue.after",Fun);
		Value* cond  = Builder -> CreateICmpNE(unwrap(LLVMConstInt(wrap(IntegerType::get(getGlobalContext(),32)), 1, 1)), unwrap(LLVMConstInt(wrap(IntegerType::get(getGlobalContext(),32)), 0, 1)),"");
		Builder -> CreateCondBr(cond, continue_block, continue_after, NULL);
		Builder -> SetInsertPoint(continue_block);
		if(info.body == info.reinit) Builder -> CreateBr(info.expr);
		else Builder -> CreateBr(info.reinit);
		Builder -> SetInsertPoint(continue_after);
	}
};

selection_stmt:		  
		          IF
{
	BasicBlock *if_join = BasicBlock::Create(getGlobalContext(),"if.join",Fun);
	$<bb>$ = if_join;
	symbol_push_scope(); //right?
} LPAREN expression RPAREN   /* if expr is pointer, do not create 0 with it, because null is uncomparable. convert to integet then compare it with 0*/
{
  	BasicBlock *if_then = BasicBlock::Create(getGlobalContext(),"if.then",Fun);
	BasicBlock *if_else = BasicBlock::Create(getGlobalContext(),"if.else",Fun);
	if(!LLVMIsAConstantInt(wrap($4)))
	{
		$4 = Builder -> CreatePtrToInt($4,unwrap(LLVMInt32Type()), "");	
	}
	Value * zero = unwrap(LLVMConstInt(LLVMInt32Type(), 0, 1));
	Value * cond  = Builder -> CreateICmpNE($4, zero,"");
	Builder -> CreateCondBr(cond, if_then, if_else, NULL);

	Builder -> SetInsertPoint(if_then);
	$<bb>$ = if_else;
}
 statement
{
	Builder -> CreateBr($<bb>2);
	Builder -> SetInsertPoint($<bb>6);
} ELSE statement 
{ 
	symbol_pop_scope(); //right?
	Builder -> CreateBr($<bb>2);
	Builder -> SetInsertPoint($<bb>2);
}
;

iteration_stmt:		  
WHILE
{
	BasicBlock *while_cond = BasicBlock::Create(getGlobalContext(), "while.cond", Fun);
	BasicBlock *while_body = BasicBlock::Create(getGlobalContext(), "while.body", Fun);
	BasicBlock *while_join = BasicBlock::Create(getGlobalContext(), "while.join", Fun);
	push_loop(while_cond, while_body, while_body, while_join);
	Builder -> CreateBr(while_cond);
	Builder -> SetInsertPoint(while_cond);
	symbol_push_scope(); //right?
}LPAREN expression RPAREN { 
	loop_info_t info = get_loop();
	Value* cond  = Builder -> CreateICmpNE($4, unwrap(LLVMConstInt(wrap($4->getType()), 0, 1)),"");
	Builder -> CreateCondBr(cond, info.body, info.exit, NULL);

	Builder -> SetInsertPoint(info.body);
} 
  statement
{
	loop_info_t info = get_loop();
	Builder -> CreateBr(info.expr);
	Builder -> SetInsertPoint(info.exit);
	pop_loop();
	symbol_pop_scope(); //right?
}

| FOR
{
	symbol_push_scope(); //right?
	BasicBlock *for_cond = BasicBlock::Create(getGlobalContext(), "for.cond", Fun);
	BasicBlock *for_body = BasicBlock::Create(getGlobalContext(), "for.body", Fun);
	BasicBlock *for_reinit = BasicBlock::Create(getGlobalContext(), "for.reinit", Fun);
	BasicBlock *for_join = BasicBlock::Create(getGlobalContext(), "for.join", Fun);
	push_loop(for_cond, for_body, for_reinit, for_join);
} LPAREN expr_opt SEMICOLON
{
	loop_info_t info = get_loop();
	Builder -> CreateBr(info.expr);
	Builder -> SetInsertPoint(info.expr);
} expr_opt SEMICOLON
{
	loop_info_t info = get_loop();
	Value* cond  = Builder -> CreateICmpNE($7, unwrap(LLVMConstInt(wrap($7->getType()), 0, 1)),"");
	Builder -> CreateCondBr(cond, info.body, info.exit, NULL);
	Builder -> SetInsertPoint(info.reinit);
	
} expr_opt RPAREN
{
	loop_info_t info = get_loop();
	Builder -> CreateBr(info.expr);
	Builder -> SetInsertPoint(info.body);
} statement
{
	loop_info_t info = get_loop();
	Builder -> CreateBr(info.reinit);
	Builder -> SetInsertPoint(info.exit);
	pop_loop();
	symbol_pop_scope(); //right?
}
;

expr_opt:		
{ 
	$$ = unwrap(LLVMConstInt(LLVMInt32Type(), 1, 0));	// right? need to make (Const *)NULL ?	
}
			| expression
{
	$$ = $1; 
}
;

jump_stmt:		  RETURN SEMICOLON
{ 
        BasicBlock *return_block = BasicBlock::Create(getGlobalContext(),"return.",Fun);
        BasicBlock *return_after = BasicBlock::Create(getGlobalContext(),"return.after",Fun);
        Value* cond  = Builder -> CreateICmpNE(unwrap(LLVMConstInt(wrap(IntegerType::get(getGlobalContext(),32)), 1, 1)), unwrap(LLVMConstInt(wrap(IntegerType::get(getGlobalContext(),32)), 0, 1)),"");
        Builder -> CreateCondBr(cond, return_block, return_after, NULL);
        Builder -> SetInsertPoint(return_block);
	Builder->CreateRetVoid();
        Builder -> SetInsertPoint(return_after);
}
			| RETURN expression SEMICOLON
{
        BasicBlock *return_block = BasicBlock::Create(getGlobalContext(),"return.",Fun);
        BasicBlock *return_after = BasicBlock::Create(getGlobalContext(),"return.after",Fun);
        Value* cond  = Builder -> CreateICmpNE(unwrap(LLVMConstInt(wrap(IntegerType::get(getGlobalContext(),32)), 1, 1)), unwrap(LLVMConstInt(wrap(IntegerType::get(getGlobalContext(),32)), 0, 1)),"");
        Builder -> CreateCondBr(cond, return_block, return_after, NULL);
        Builder -> SetInsertPoint(return_block);
	Builder->CreateRet($2);
        Builder -> SetInsertPoint(return_after);
}
;

expression:               assignment_expression
{ 
  $$=$1;
}
;

assignment_expression:    conditional_expression
{
  $$=$1;
}
                        | lhs_expression ASSIGN assignment_expression
{
	$$ = Builder -> CreateLoad($1, "");
	if($$ -> getType() != $3 -> getType())
	{
		if(LLVMGetTypeKind(LLVMTypeOf(wrap($3))) == LLVMPointerTypeKind)
		{
			$3 = Builder -> CreatePtrToInt($3,$$->getType(), "");
		}
		else
		{
			$3 = Builder -> CreateIntToPtr($3,$$->getType(), "");
		}
	}
	$$ = Builder -> CreateStore($3, $1, false);
}
;


conditional_expression:   logical_OR_expression
{
  $$=$1;
}
                        | logical_OR_expression QUESTION_MARK expression COLON conditional_expression
{
	Value * zero = unwrap(LLVMConstInt(LLVMInt32Type(), 0, 1));
	if(!LLVMIsAConstantInt(wrap($1)))
	{
		$1 = Builder -> CreatePtrToInt($1,unwrap(LLVMInt32Type()), "");	
	}
	
	$$ = Builder -> CreateSelect(Builder -> CreateICmpNE($1, zero, ""), $3, $5, "");
}
;

logical_OR_expression:    logical_AND_expression
{
  $$ = $1;
}
                        | logical_OR_expression LOGICAL_OR logical_AND_expression
{
	Value * zero = unwrap(LLVMConstInt(LLVMInt32Type(), 0, 1));
	if(!LLVMIsAConstantInt(wrap($1)))
	{
		$1 = Builder -> CreatePtrToInt($1,unwrap(LLVMInt32Type()), "");	
	}
	if(!LLVMIsAConstantInt(wrap($3)))
	{
		$3 = Builder -> CreatePtrToInt($3,unwrap(LLVMInt32Type()), "");	
	}
		
	$$ = Builder -> CreateZExt(Builder -> CreateOr(Builder -> CreateICmpNE($1, zero, ""), Builder -> CreateICmpNE($3, zero, ""),""), unwrap(LLVMInt32Type()) , "");

};

logical_AND_expression:   inclusive_OR_expression
{
  $$ = $1;
}
                        | logical_AND_expression LOGICAL_AND inclusive_OR_expression
{
	Value * zero = unwrap(LLVMConstInt(LLVMInt32Type(), 0, 1));
	if(!LLVMIsAConstantInt(wrap($1)))
	{
		$1 = Builder -> CreatePtrToInt($1,unwrap(LLVMInt32Type()), "");	
	}
	if(!LLVMIsAConstantInt(wrap($3)))
	{
		$3 = Builder -> CreatePtrToInt($3,unwrap(LLVMInt32Type()), "");	
	}
		
	$$ = Builder -> CreateZExt(Builder -> CreateAnd(Builder -> CreateICmpNE($1, zero, ""), Builder -> CreateICmpNE($3, zero, ""),""), unwrap(LLVMInt32Type()) , "");
}
;

inclusive_OR_expression:  exclusive_OR_expression
{
    $$=$1;
}
                        | inclusive_OR_expression BITWISE_OR exclusive_OR_expression
{
	if(!LLVMIsAConstantInt(wrap($1)))
	{
		$1 = Builder -> CreatePtrToInt($1,unwrap(LLVMInt32Type()), "");	
	}
	if(!LLVMIsAConstantInt(wrap($3)))
	{
		$3 = Builder -> CreatePtrToInt($3,unwrap(LLVMInt32Type()), "");	
	}
	$$ = Builder -> CreateOr($1, $3, "");
}
;

exclusive_OR_expression:  AND_expression
{
  $$ = $1;
}
                        | exclusive_OR_expression BITWISE_XOR AND_expression
{
	if(!LLVMIsAConstantInt(wrap($1)))
	{
		$1 = Builder -> CreatePtrToInt($1,unwrap(LLVMInt32Type()), "");	
	}
	if(!LLVMIsAConstantInt(wrap($3)))
	{
		$3 = Builder -> CreatePtrToInt($3,unwrap(LLVMInt32Type()), "");	
	}
	$$ = Builder -> CreateXor($1, $3, "");
}
;

AND_expression:           equality_expression
{
  $$ = $1;
}
                        | AND_expression BITWISE_AND equality_expression
{
	if(!LLVMIsAConstantInt(wrap($1)))
	{
		$1 = Builder -> CreatePtrToInt($1,unwrap(LLVMInt32Type()), "");	
	}
	if(!LLVMIsAConstantInt(wrap($3)))
	{
		$3 = Builder -> CreatePtrToInt($3,unwrap(LLVMInt32Type()), "");	
	}
	$$ = Builder -> CreateAnd($1, $3, "");
}
;

equality_expression:      relational_expression
{
  $$ = $1;
}
                        | equality_expression EQ relational_expression
{
	if($1 -> getType() != $3 -> getType())
	{
		if(LLVMGetTypeKind(LLVMTypeOf(wrap($1))) == LLVMPointerTypeKind)
		{
			$3 = Builder -> CreateIntToPtr($3, unwrap(LLVMPointerType(LLVMInt32Type(),0)), "");
		}
		else
		{
			$1 = Builder -> CreateIntToPtr($1,unwrap(LLVMPointerType(LLVMInt32Type(),0)), "");
		}
	}
	$$ = Builder -> CreateZExt(Builder -> CreateICmpEQ($1, $3, ""), unwrap(LLVMInt32Type()), "");
}
                        | equality_expression NEQ relational_expression
{
	if($1 -> getType() != $3 -> getType())
	{
		if(LLVMGetTypeKind(LLVMTypeOf(wrap($1))) == LLVMPointerTypeKind)
		{
			$3 = Builder -> CreateIntToPtr($3, unwrap(LLVMPointerType(LLVMInt32Type(),0)), "");
		}
		else
		{
			$1 = Builder -> CreateIntToPtr($1,unwrap(LLVMPointerType(LLVMInt32Type(),0)), "");
		}
	}
	$$ = Builder -> CreateZExt(Builder -> CreateICmpNE($1, $3, ""), unwrap(LLVMInt32Type()), "");
}
;

relational_expression:    shift_expression
{
    $$=$1;
}
                        | relational_expression LT shift_expression
{
	if($1 -> getType() != $3 -> getType())
	{
		if(LLVMGetTypeKind(LLVMTypeOf(wrap($1))) == LLVMPointerTypeKind)
		{
			$3 = Builder -> CreateIntToPtr($3, unwrap(LLVMPointerType(LLVMInt32Type(),0)), "");
		}
		else
		{
			$1 = Builder -> CreateIntToPtr($1, unwrap(LLVMPointerType(LLVMInt32Type(),0)), "");
		}
	}
	$$ = Builder -> CreateZExt(Builder -> CreateICmpSLT($1, $3, ""), unwrap(LLVMInt32Type()), "");
}
                        | relational_expression GT shift_expression
{
	if($1 -> getType() != $3 -> getType())
	{
		if(LLVMGetTypeKind(LLVMTypeOf(wrap($1))) == LLVMPointerTypeKind)
		{
			$3 = Builder -> CreateIntToPtr($3, unwrap(LLVMPointerType(LLVMInt32Type(),0)), "");
		}
		else
		{
			$1 = Builder -> CreateIntToPtr($1, unwrap(LLVMPointerType(LLVMInt32Type(),0)), "");
		}
	}
	$$ = Builder -> CreateZExt(Builder -> CreateICmpSGT($1, $3, ""), unwrap(LLVMInt32Type()), "");
}
                        | relational_expression LE shift_expression
{
	if($1 -> getType() != $3 -> getType())
	{
		if(LLVMGetTypeKind(LLVMTypeOf(wrap($1))) == LLVMPointerTypeKind)
		{
			$3 = Builder -> CreateIntToPtr($3, unwrap(LLVMPointerType(LLVMInt32Type(),0)), "");
		}
		else
		{
			$1 = Builder -> CreateIntToPtr($1,unwrap(LLVMPointerType(LLVMInt32Type(),0)), "");
		}
	}
	$$ = Builder -> CreateZExt(Builder -> CreateICmpSLE($1, $3, ""), unwrap(LLVMInt32Type()), "");
}
                        | relational_expression GE shift_expression
{
	if($1 -> getType() != $3 -> getType())
	{
		if(LLVMGetTypeKind(LLVMTypeOf(wrap($1))) == LLVMPointerTypeKind)
		{
			$3 = Builder -> CreateIntToPtr($3, unwrap(LLVMPointerType(LLVMInt32Type(),0)), "");
		}
		else
		{
			$1 = Builder -> CreateIntToPtr($1, unwrap(LLVMPointerType(LLVMInt32Type(),0)), "");
		}
	}
	$$ = Builder -> CreateZExt(Builder -> CreateICmpSGE($1, $3, ""), unwrap(LLVMInt32Type()), "");
}
;

shift_expression:         additive_expression
{
    $$=$1;
}
                        | shift_expression LSHIFT additive_expression
{
	if(!LLVMIsAConstantInt(wrap($1)))
	{
		$1 = Builder -> CreatePtrToInt($1,unwrap(LLVMInt32Type()), "");	
	}
	if(!LLVMIsAConstantInt(wrap($3)))
	{
		$3 = Builder -> CreatePtrToInt($3,unwrap(LLVMInt32Type()), "");	
	}
	$$ = Builder -> CreateShl($1, $3, "", false, false);
}
                        | shift_expression RSHIFT additive_expression
{
	if(!LLVMIsAConstantInt(wrap($1)))
	{
		$1 = Builder -> CreatePtrToInt($1,unwrap(LLVMInt32Type()), "");	
	}
	if(!LLVMIsAConstantInt(wrap($3)))
	{
		$3 = Builder -> CreatePtrToInt($3,unwrap(LLVMInt32Type()), "");	
	}
	$$ = Builder -> CreateLShr($1, $3, "", false);
}
;

additive_expression:      multiplicative_expression
{
  $$ = $1;
}
                        | additive_expression PLUS multiplicative_expression
{
	if ((LLVMGetTypeKind(LLVMTypeOf(wrap($1))) == LLVMPointerTypeKind ) || (LLVMGetTypeKind(LLVMTypeOf(wrap($3))) == LLVMPointerTypeKind ))
	{
		if(LLVMGetTypeKind(LLVMTypeOf(wrap($1))) == LLVMPointerTypeKind )
		{
			$$ = Builder -> CreateGEP($1,$3 , "");
		}
		if(LLVMGetTypeKind(LLVMTypeOf(wrap($3))) == LLVMPointerTypeKind )
		{
			$$ = Builder -> CreateGEP($3,$1 , "");
		}
	}	
	else $$ = Builder -> CreateAdd($1, $3, "", false, false);
}
                        | additive_expression MINUS multiplicative_expression
{
	if ((LLVMGetTypeKind(LLVMTypeOf(wrap($1))) == LLVMPointerTypeKind ) || (LLVMGetTypeKind(LLVMTypeOf(wrap($3))) == LLVMPointerTypeKind ))
	{
		if(LLVMGetTypeKind(LLVMTypeOf(wrap($1))) == LLVMPointerTypeKind )
		{
			$3 = Builder -> CreateNeg($3, "", false, false);
			$$ = Builder -> CreateGEP($1,$3 , "");
		}
	}	
	else $$ = Builder -> CreateSub($1, $3, "", false, false);
}
;

multiplicative_expression:  cast_expression
{
  $$ = $1;
}
                        | multiplicative_expression STAR cast_expression
{
	if($1 -> getType() != $3 -> getType())
	{
		num_errors ++;
		parser_error("STAR Type don't match!");
		exit(1);
	}
	$$ = Builder -> CreateMul($1, $3, "", false, false);
}
                        | multiplicative_expression DIV cast_expression
{
	if($1 -> getType() != $3 -> getType())
	{
		num_errors ++;
		parser_error("DIV Type don't match!");
		exit(1);
	}
	$$ = Builder -> CreateSDiv($1, $3, "", false);
}
                        | multiplicative_expression MOD cast_expression
{
	if($1 -> getType() != $3 -> getType())
	{
		num_errors ++;
		parser_error("MOD Type don't match!");
		exit(1);
	}
	$$ = Builder -> CreateSRem($1, $3, "");
}
;

cast_expression:          unary_expression
{ $$ = $1; }
;

lhs_expression:           IDENT 
{
  int isArg=0;
  Value* val = symbol_find($1,&isArg);
  if (isArg)
    {
	num_errors++;
	parser_error("Undefined variable.");
	exit(1);
    }
  else
    $$ = val;
}
                        | STAR IDENT
{
  int isArg=0;
  Value* val = symbol_find($2,&isArg);
  if (isArg)
    {
	num_errors++;
	parser_error("Undefined variable.");
	exit(1);
    }
  else
    $$ = Builder->CreateLoad(val);
}
;

unary_expression:         postfix_expression
{
  $$ = $1;
}
                        | BITWISE_AND primary_expression %prec UAND //judge if it is load insruction.
{
	if(LLVMIsALoadInst(wrap($2)))
	{
		$$ = unwrap(LLVMGetOperand(wrap($2), 0));
	} 
	else
	{
		num_errors ++;
		parser_error("The operand is not an address.");
		exit(1);
	}
}
                        | STAR primary_expression %prec USTAR
{	
	if(LLVMGetTypeKind(LLVMTypeOf(wrap($2))) != LLVMPointerTypeKind)
	{
		$2 = Builder -> CreateIntToPtr($2, unwrap(LLVMPointerType(LLVMInt32Type(),0)),"");
	}
  $$ = Builder -> CreateLoad($2, "");
}
                        | MINUS unary_expression %prec UMINUS
{
	$$ = Builder -> CreateNeg($2, "", false, false);
}
                        | PLUS unary_expression %prec UPLUS
{
  $$ = $2;
}
                        | BITWISE_INVERT unary_expression
{
	if(!LLVMIsAConstantInt(wrap($2)))
	{
		$2 = Builder -> CreatePtrToInt($2,unwrap(LLVMInt32Type()), "");	
	}
	$$ = Builder -> CreateXor($2, unwrap(LLVMConstInt(LLVMInt32Type(),0xFFFFFFFF,1)), "");
}
                        | NOT unary_expression
{
	if(!LLVMIsAConstantInt(wrap($2)))
	{
		$2 = Builder -> CreatePtrToInt($2,unwrap(LLVMInt32Type()), "");	
	}
	Value * zero = unwrap(LLVMConstInt(LLVMInt32Type(), 0, 1));
	$$ = Builder -> CreateZExt(Builder -> CreateICmpEQ($2, zero, ""), unwrap(LLVMInt32Type()), "");
}
;


postfix_expression:       primary_expression
{
  $$ = $1;
}
;

primary_expression:       IDENT 
{ 
  int isArg=0;
  Value* val = symbol_find($1,&isArg);
  if (isArg)
    $$ = val;
  else
    $$ = Builder->CreateLoad(val);
}
                        | constant
{
	$$ = unwrap(LLVMConstInt(LLVMInt32Type(), $1, 0));
}
                        | LPAREN expression RPAREN
{
  $$ = $2;
}
;

constant:	          NUMBER  
{ 
	$$ = $1;
} 
;

%%

Value* BuildFunction(Type* RetType, const char *name, 
			   paramlist_t *params)
{
  int i;
  int size = paramlist_size(params);

  std::vector<Type*> v;
  std::vector<const char*> vname;

  v.resize(size,NULL);
  vname.resize(size,NULL);

  paramlist_t *tmp = params;
  i=size-1;
  while(tmp)
    {
      v[i]=(tmp->type);
      vname[i]=(tmp->name);
      tmp = next_param(tmp);
      i--;
    }
  ArrayRef<Type*> Params(v);

  FunctionType* FunType = FunctionType::get(RetType,Params,false);

  Fun = Function::Create(FunType,GlobalValue::ExternalLinkage,
			 name,M);
  Twine T("entry");
  BasicBlock *BB = BasicBlock::Create(getGlobalContext(),T,Fun);

  Builder = new IRBuilder<>(getGlobalContext());
  Builder->SetInsertPoint(BB);

  Function::arg_iterator I = Fun->arg_begin();
  for(i=0; I!=Fun->arg_end();i++, I++)
    {
      AllocaInst *AI = Builder->CreateAlloca(v[i]);
      Builder->CreateStore(&(*I),(Value*)AI);
      symbol_insert(vname[i],(Value*)AI,0);
    }


  return Fun;
}

extern int line_num;
extern char *infile[];
static int   infile_cnt=0;
extern FILE * yyin;

int parser_error(const char *msg)
{
  printf("%s (%d): Error -- %s\n",infile[infile_cnt-1],line_num,msg);
  return 1;
}

int internal_error(const char *msg)
{
  printf("%s (%d): Internal Error -- %s\n",infile[infile_cnt-1],line_num,msg);
  return 1;
}

int yywrap() {
  static FILE * currentFile = NULL;

  if ( (currentFile != 0) ) {
    fclose(yyin);
  }
  
  if(infile[infile_cnt]==NULL)
    return 1;

  currentFile = fopen(infile[infile_cnt],"r");
  if(currentFile!=NULL)
    yyin = currentFile;
  else
    printf("Could not open file: %s",infile[infile_cnt]);

  infile_cnt++;
  
  return (currentFile)?0:1;
}

int yyerror(const char* error)
{
  parser_error("Un-resolved syntax error.");
  return 1;
}

char * get_filename()
{
  return infile[infile_cnt-1];
}

int get_lineno()
{
  return line_num;
}


void minic_abort()
{
  parser_error("Too many errors to continue.");
  exit(1);
}
