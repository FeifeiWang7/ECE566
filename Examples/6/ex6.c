#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>

#include "llvm-c/Core.h"
#include "llvm-c/Target.h"
#include "llvm-c/BitReader.h"
#include "llvm-c/BitWriter.h"

#include "valmap.h"

void Error(LLVMValueRef V, FILE *out) {
  char *str = LLVMPrintValueToString(V);
  size_t size = strlen(str);
  if(str[size-1]=='\n')
    str[size-1]=0;

  printf("Error-not handled: (%s)\n",str);  
  fprintf(out,"<Error>: %s\n",str);
}

LLVMTargetDataRef TD;

/* Calculate size of the stack */
unsigned long long CalcLocalSize(LLVMValueRef F)
{
  unsigned long long sum=0;
  LLVMValueRef I;
  LLVMBasicBlockRef BB;
  for(BB=LLVMGetFirstBasicBlock(F);
      BB!=NULL;
      BB=LLVMGetNextBasicBlock(BB))
    {
      for(I=LLVMGetFirstInstruction(BB);
	  I!=NULL;
	  I=LLVMGetNextInstruction(I))
	{
	  if (LLVMIsAAllocaInst(I)) 
	    {
	      LLVMTypeRef ty = LLVMTypeOf(I);
	      LLVMTypeRef e = LLVMGetElementType(ty);
	      sum += LLVMStoreSizeOfType(TD,e);	      
	    }
	}      
    }
  return sum;
}

// Global variable to track symbols, it maps:
//   LLVMValueRef -> Offset
//   If it's an alloca, interpret as offset as from R5
//   It it's a global, interpret offset as if from R4
valmap_t symbols;

// Compute entries in symbol table
valmap_t SymbolTable(LLVMModuleRef M)
{
  valmap_t map = valmap_create();

  LLVMValueRef F;
  LLVMValueRef I;
  LLVMBasicBlockRef BB;

  unsigned long long offset = 0;

  for(F=LLVMGetFirstFunction(M);
      F!=NULL;
      F=LLVMGetNextFunction(F))
    {
      if (LLVMGetFirstBasicBlock(F)==NULL)
	continue;
     
      unsigned long long retSize = LLVMStoreSizeOfType(TD,LLVMTypeOf(F)); 
      offset=retSize+8;
      int p;
      for(p=0; p<LLVMCountParams(F); p++)
	{
	  LLVMValueRef a = LLVMGetParam(F,p);
	  LLVMTypeRef ty = LLVMTypeOf(a);
	  unsigned long long size = LLVMStoreSizeOfType(TD,ty);
	  valmap_insert(map,I,(void*)offset);
	  offset+=size;	  
	}

      offset=0;
      for(BB=LLVMGetFirstBasicBlock(F);
	  BB!=NULL;
	  BB=LLVMGetNextBasicBlock(BB))
	{
	  for(I=LLVMGetFirstInstruction(BB);
	      I!=NULL;
	      I=LLVMGetNextInstruction(I))
	    {
	      if (LLVMIsAAllocaInst(I)) 
		{
		  LLVMTypeRef ty = LLVMTypeOf(I);
		  LLVMTypeRef e = LLVMGetElementType(ty);
		  unsigned long long size = LLVMStoreSizeOfType(TD,e);
		  valmap_insert(map,I,(void*)offset-size);
		  offset-=size;		  
		}
	    }      
	}

    }

  LLVMValueRef G;
  offset = 0;
  for(G=LLVMGetFirstGlobal(M);
      G!=NULL;
      G=LLVMGetNextGlobal(G))
    {
      LLVMTypeRef ty = LLVMTypeOf(G);
      LLVMTypeRef e = LLVMGetElementType(ty);
      unsigned long long size = LLVMStoreSizeOfType(TD,e);
      valmap_insert(map,I,(void*)offset);
      offset+=size;		        
    }

  return map;
}

long regCnt=8;
valmap_t reg;
void GenValue(LLVMValueRef V, FILE *out);

void GenICmp(LLVMValueRef V, FILE *out)
{
  for(int i=0; i<LLVMGetNumOperands(V); i++) {
    GenValue(LLVMGetOperand(V,i), out);
  }

  long tmp, label;
  LLVMIntPredicate pred = LLVMGetICmpPredicate(V);
  switch(pred) {
  case LLVMIntSLT:     /**< signed less than */   
    fprintf(out,"   LI  R%ld,0       # get temporary register \n",tmp=regCnt++);
    fprintf(out,"   SUB R%ld,R%ld,R%ld # icmp slt \n",regCnt++,(long)valmap_find(reg,LLVMGetOperand(V,0)),(long)valmap_find(reg,LLVMGetOperand(V,1)));
    fprintf(out,"   BRzp L%ld          #\n",label=regCnt++);
    fprintf(out,"   ADD R%ld, R%ld, 1  #\n",tmp,tmp);
    fprintf(out,"L%ld:                #\n",label);
    break;
  case LLVMIntEQ: 
  case LLVMIntNE:      /**< not equal */
  case LLVMIntUGT:     /**< unsigned greater than */
  case LLVMIntUGE:     /**< unsigned greater or equal */
  case LLVMIntULT:     /**< unsigned less than */
  case LLVMIntULE:     /**< unsigned less or equal */
  case LLVMIntSGT:     /**< signed greater than */
  case LLVMIntSGE:     /**< signed greater or equal */
  case LLVMIntSLE:
  default:
    fprintf(out, "   <ERROR- %d/%d not implemented>\n",pred,LLVMIntSLT);
    LLVMDumpValue(V);
    break;
  }

  valmap_insert(reg,V,(void*)tmp);
}

LLVMValueRef LLVMGetCalledFunction(LLVMValueRef);

void GenCall(LLVMValueRef V, FILE *out)
{
  for(int i=0; i<LLVMGetNumOperands(V); i++) {
    GenValue(LLVMGetOperand(V,i), out);
  }

  for(int i=LLVMGetNumOperands(V)-1; i>=0; i--) {
    // push each arg onto stack
    LLVMValueRef op = LLVMGetOperand(V,i);
    fprintf(out,"   ADD R6, R6, -%llu \n", LLVMStoreSizeOfType(TD,LLVMTypeOf(op)));
    fprintf(out,"   STR R%ld, R6,  0 \n", (long)valmap_find(reg,op));
  }

  LLVMValueRef F = LLVMGetCalledFunction(V);

  if (!LLVMIsAInstruction(F)) {
    fprintf(out,"   JSR %s \n", LLVMGetValueName(F));
  } else {
    GenValue(F,out);
    fprintf(out,"   JSRR R%ld \n", (long)valmap_find(reg,F));
  }

  fprintf(out,"   LDR R%ld, R6, 0 \n", regCnt);
  fprintf(out,"   ADD R6, R6, %llu \n", LLVMStoreSizeOfType(TD,LLVMTypeOf(V)));
  valmap_insert(reg,V,(void*)(regCnt++));
}

void GenGEP(LLVMValueRef V, FILE *out)
{
  for(int i=0; i<LLVMGetNumOperands(V); i++) {
    GenValue(LLVMGetOperand(V,i), out);
  }
    
  long r = (long)valmap_find(reg,LLVMGetOperand(V,0));
  long long size=0;
  LLVMTypeRef type = LLVMTypeOf(LLVMGetOperand(V,0));

  for(int i=1; i<LLVMGetNumOperands(V); i++) {
    LLVMValueRef op = LLVMGetOperand(V,i);
    
    if (LLVMGetTypeKind(type)==LLVMPointerTypeKind) {      
      type = LLVMGetElementType(type);
    } else if (LLVMGetTypeKind(type)==LLVMArrayTypeKind) {	
      type = LLVMGetElementType(type);	
    } else {
      long long c = LLVMConstIntGetSExtValue(op);
      LLVMTypeRef dest[512];
      LLVMGetStructElementTypes(type,dest);
      type = dest[(int)c];
    }      
    
    if (LLVMIsAConstantInt(op)) {
      long long c = LLVMConstIntGetSExtValue(op);
      size += c * LLVMStoreSizeOfType(TD, LLVMGetElementType(type));          
    } else {
      if (size>0) {
	fprintf(out,"   ADD R%ld, R%ld, %lld\n",regCnt,r,size);
	r = regCnt++;
	size = 0;
      }
      fprintf(out,"   MUL R%ld, R%ld, %llu\n", regCnt, (long)valmap_find(reg,op), LLVMStoreSizeOfType(TD, LLVMGetElementType(type)));
      fprintf(out,"   ADD R%ld, R%ld, R%ld\n",regCnt,regCnt,r);
      r = regCnt++;    
    }
  }

  if (size > 0) {
    fprintf(out,"   ADD R%ld, R%ld, %lld\n",regCnt,r, size);
    r = regCnt++;
  }
  valmap_insert(reg,V,(void*)r);
}

void GenBranch(LLVMValueRef V, FILE *out)
{
  if (LLVMGetNumOperands(V)==1) {
    GenValue(LLVMGetOperand(V,0),out);
    
    fprintf(out,"   BRnzp L%d\n",(int)valmap_find(reg,LLVMGetOperand(V,0)));
  } else {
    GenValue(LLVMGetOperand(V,1),out);
    GenValue(LLVMGetOperand(V,2),out);
    fprintf(out,"   ADD R%ld, R%ld, 0 # set condition codes\n", regCnt, (long)valmap_find(reg,LLVMGetOperand(V,0)));
    fprintf(out,"   BRnp L%d\n",(int)valmap_find(reg,LLVMGetOperand(V,1)));
    fprintf(out,"   BRz  L%d\n",(int)valmap_find(reg,LLVMGetOperand(V,2)));
  }
}

// Generate instruction to compute address of global
void GenGlobalVar(LLVMValueRef V, FILE *out)
{
  fprintf(out,"   ADD R%ld, R4, %ld",regCnt,(long)valmap_find(symbols,V));
  valmap_insert(reg,V,(void*)regCnt++);
}

// Insert label for block into reg map
void GenBasicBlock(LLVMValueRef V, FILE *out)
{
  // We just need to compute the label number
  if (!valmap_check(reg,V))
    valmap_insert(reg,V,(void*)(regCnt++));
}

void GenConstantInt(LLVMValueRef V, FILE *out)
{
  // Load constant into a register
    fprintf(out,"   LI R%ld, %lld\n",regCnt, LLVMConstIntGetSExtValue(V));
    valmap_insert(reg,V,(void*)(regCnt++));
}

void GenAllocaInst(LLVMValueRef V, FILE *out)
{
  int offset = (int) valmap_find(symbols,V);	
  fprintf(out,"   ADD R%ld, R5, %d\n", regCnt, offset);
  valmap_insert(reg, V, (void*)regCnt++);
}

void GenStoreInst(LLVMValueRef V, FILE *out)
{
  GenValue(LLVMGetOperand(V,0),out);
  LLVMValueRef addr = LLVMGetOperand(V,1);
  if ( LLVMIsAAllocaInst(addr) || LLVMIsAGlobalVariable(addr) ) {
    int offset = (int) valmap_find(symbols,addr);	


    if (LLVMIsAAllocaInst(addr))
      // if comes form alloca, use R5
      fprintf(out,"   STR R%ld, R5, %d\n", 
	      (long)valmap_find(reg,LLVMGetOperand(V,0)), offset);
    else
      // else use R4
      fprintf(out,"   STR R%ld, R4, %d\n", regCnt, offset);   

  } else {
    GenValue(LLVMGetOperand(V,1),out);    
    Error(V,out);
  }
}

void GenLoadInst(LLVMValueRef V, FILE *out)
{
  LLVMValueRef addr = LLVMGetOperand(V,0);
  if ( LLVMIsAAllocaInst(addr) || LLVMIsAGlobalVariable(addr) ) {
    int offset = (int) valmap_find(symbols,addr);	
    if (LLVMIsAAllocaInst(addr))
      fprintf(out,"   LDR R%ld, R5, %d\n", regCnt, offset);
    else
      fprintf(out,"   LDR R%ld, R4, %d\n", regCnt, offset);
    
    valmap_insert(reg,V,(void*)(regCnt++));	
  } else {
    for(int i=0; i<LLVMGetNumOperands(V); i++) {
      GenValue(LLVMGetOperand(V,i), out);
    }
    fprintf(out,"   LDR R%ld, R%d, %d\n", regCnt,(int)valmap_find(reg,LLVMGetOperand(V,0)),0);
    valmap_insert(reg,V,(void*)(regCnt++));
  }
}

void GenAddInst(LLVMValueRef V, FILE *out)
{
  fprintf(out,"   ADD R%ld",regCnt);
  for(int i=0; i<LLVMGetNumOperands(V); i++) {
    int r = (int)valmap_find(reg,LLVMGetOperand(V,i));
    fprintf(out,", R%d",r);
  }
  valmap_insert(reg,V,(void*)(regCnt++));
  fprintf(out,"\n");
}

void GenRetInst(LLVMValueRef V, FILE *out)
{
  if (LLVMGetNumOperands(V)==1) {
    GenValue(LLVMGetOperand(V,0),out);
    // should not actually be 12, but rather a calculation
    fprintf(out, "   STR R%d, R5, %d \n",(int)valmap_find(reg,LLVMGetOperand(V,0)), 12);
  } 	
  fprintf(out,"   BR EPI   #branch to epilogue\n");	     
}

void GenValue(LLVMValueRef V, FILE *out)
{
  // Don't remember these across instructions
  if (LLVMIsAConstantArray(V)) {
    Error(V,out);
    return;
  } else if (LLVMIsAGlobalVariable(V)) {
    GenGlobalVar(V,out);
    return;
  } else if (LLVMIsABasicBlock(V)) {
    GenBasicBlock(V,out);
    return;
  } else if (LLVMIsAConstantInt(V)) {
    GenConstantInt(V,out);
    return;
  }

  // Remember instructions... if we've already gen'd code for it,
  // don't do it again ... SSA form guarantees we have a legal
  // register available
  if (valmap_check(reg,V))
    return;
  
  if (LLVMIsAAllocaInst(V)) {
    GenAllocaInst(V,out);
    return;
  } else if (LLVMIsALoadInst(V)) {
    GenLoadInst(V,out);
    return; 
  } else if (LLVMIsAStoreInst(V)) {
    GenStoreInst(V,out);
    return;
  }

  // Make sure operands have been generated, this mostly handles the
  // case of basic blocks, constants, and global addresses that will not
  // yet be in a register or have a label since they are not instructions
  for(int i=0; i<LLVMGetNumOperands(V); i++) {
    GenValue(LLVMGetOperand(V,i), out);
  }


  // Handle remaining instructions
  if (LLVMIsAInstruction(V)) {    
    LLVMOpcode opcode = LLVMGetInstructionOpcode(V);
    switch(opcode) {
    case LLVMAdd:
      GenAddInst(V,out);
      break;
    case LLVMRet:
      GenRetInst(V,out);
      break;
    case LLVMBr: 
      GenBranch(V,out);
      break;     
    case LLVMICmp:
      GenICmp(V,out);
      break;
    case LLVMCall:
      GenCall(V,out);
      break;
    case LLVMGetElementPtr:
      GenGEP(V,out);
      break;
    default:
      Error(V,out);
      break;
    }
  } else {
    // Anyting else we missed
    Error(V,out);
  }
}

void GenBody(LLVMValueRef F, FILE *out)
{
  LLVMValueRef I;
  LLVMBasicBlockRef BB;
  for(BB=LLVMGetFirstBasicBlock(F);
      BB!=NULL;
      BB=LLVMGetNextBasicBlock(BB))
    {
      if ( !valmap_check(reg,LLVMBasicBlockAsValue(BB)) ) {
	fprintf(out,"L%ld:\n",regCnt);
	valmap_insert(reg,LLVMBasicBlockAsValue(BB),(void*)(regCnt++));
      } else {
	fprintf(out,"L%d:\n",(int)valmap_find(reg,LLVMBasicBlockAsValue(BB)));
      }

      for(I=LLVMGetFirstInstruction(BB);
	  I!=NULL;
	  I=LLVMGetNextInstruction(I))
	{	  	  
	  GenValue(I,out);
	}      
    }
}

// Generate code for LC3
void CodeGen(LLVMModuleRef Module, FILE *out) 
{
  fprintf(out,"# LC-3 Assembly file\n");
  fprintf(out,"# Generated by Example 6, a tool built using LLVM\n");
  fprintf(out,"#\n#\n");

  //TD = LLVMCreateTargetData(LLVMGetDataLayout(Module));
  TD = LLVMCreateTargetData("e-p:32:32:32-i1:8:8-i8:8:8-i16:16:16-i32:32:32");

  symbols = SymbolTable(Module);
  reg = valmap_create();

  LLVMValueRef F;
  for(F=LLVMGetFirstFunction(Module);
      F!=NULL;
      F=LLVMGetNextFunction(F))
    {
      if (LLVMGetFirstBasicBlock(F)==NULL)
	continue;

      unsigned long long retSize = LLVMStoreSizeOfType(TD,LLVMTypeOf(F));
      unsigned long long localSize = CalcLocalSize(F);

      fprintf(out,"# Size of return value: %llu bytes\n",retSize);
      fprintf(out,"# Sizes of locals     : %llu bytes\n",localSize);
      
      fprintf(out,"%s: # begin function\n", LLVMGetValueName(F));
      
      fprintf(out,"    ADD R6, R6, -%llu #\n", retSize);
      fprintf(out,"    ADD R6, R6, -4 #\n");
      fprintf(out,"    STR R7, R6,  0 # push R7\n");
      fprintf(out,"    ADD R6, R6, -4\n");
      fprintf(out,"    STR R5, R6,  0 # push old R5\n");
      fprintf(out,"    ADD R5, R6,  0 # set new R5 \n");
      fprintf(out,"    ADD R6, R6, -%llu # make space for locals\n",localSize);
      fprintf(out,"    # Body goes here!\n");

      GenBody(F, out);

      fprintf(out,"EPI:ADD R6, R6, %llu #\n", localSize);
      fprintf(out,"    LDR R5, R6,  0 # pop R5\n");
      fprintf(out,"    ADD R6, R6,  4 #\n");
      fprintf(out,"    LDR R7, R6,  0 # pop R7\n");
      fprintf(out,"    ADD R6, R6,  4 #\n");
      fprintf(out,"    RET            #\n");
    }

  fprintf(out,"#\n# End of file\n");
}


int
main (int argc, char ** argv)
{
  if (argc < 3) {
    fprintf(stderr,"Not enough files specified.\n");
    return 0;
  }

  LLVMMemoryBufferRef Buff=NULL;
  char *outMessage=NULL;

  LLVMCreateMemoryBufferWithContentsOfFile(argv[1],&Buff,&outMessage);

  LLVMModuleRef Module=NULL;
  if (!Buff || LLVMParseBitcode(Buff,&Module,&outMessage))
    {
      /* handle error */
      printf("Error opening file: %s\n",outMessage);
      return 1;
    }
  
  FILE* out=NULL;
  if (strcmp(argv[2],"stdout")==0)
    out = stdout;
  else
    out = fopen(argv[2],"w");

  CodeGen(Module,out);
  
  return 0;
}
