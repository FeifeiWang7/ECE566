#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>

#include "llvm-c/Core.h"
#include "llvm-c/BitReader.h"
#include "llvm-c/BitWriter.h"

#include "worklist.h"
#include "gepmap.h"


int IsScalarType(LLVMTypeRef type) 
{
  LLVMTypeKind kind = LLVMGetTypeKind(type);
  switch(kind) {
  case LLVMIntegerTypeKind:
  case LLVMFloatTypeKind:
  case LLVMDoubleTypeKind:
  case LLVMHalfTypeKind:
    return 1;
  default:
    return 0;
  }
  return 0;
}

int IsLoadOrStore(LLVMValueRef gep, LLVMValueRef val)
{
  if (LLVMIsALoadInst(val)) {
    return 1;
  } else if (LLVMIsAStoreInst(val)) {
    if (LLVMGetOperand(val,1)==gep) {
      return 1;
    }
  }
  return 0;
}

int ValidIndexHelper(LLVMTypeRef type, LLVMValueRef idx[], int size)
{
  unsigned long long index = LLVMConstIntGetSExtValue(idx[0]);
  if (LLVMGetTypeKind(type)==LLVMArrayTypeKind) {
    if (index < LLVMGetArrayLength(type)) {
      if (size > 1)
	return ValidIndexHelper(LLVMGetElementType(type),idx+1,size-1);
      else
	return 1;			  
    } else {
      return 0;
    }
  } else if (LLVMGetTypeKind(type)==LLVMStructTypeKind) {
    // must be struct type
    unsigned count = LLVMCountStructElementTypes(type);
    if (index < count && count < 512) {
      if (size > 1) {
	LLVMTypeRef dest[512];
	LLVMGetStructElementTypes(type,dest);
	return ValidIndexHelper(dest[index],idx+1,size-1);
      } else
	return 1;			  
    } else {
      return 0;
    }
  } else {
    LLVMDumpType(type);
  }
  return 0;
} 

int ValidIndex(LLVMValueRef GEP)
{
  LLVMValueRef ptr = LLVMGetOperand(GEP,0);
  LLVMTypeRef type = LLVMTypeOf(ptr);
  
  LLVMValueRef *array = (LLVMValueRef*)malloc(sizeof(LLVMValueRef)*(LLVMGetNumOperands(GEP)-1));
  for(unsigned i=1; i<LLVMGetNumOperands(GEP); i++)
    array[i-1] = LLVMGetOperand(GEP,i);

  int ret=1;
  if (LLVMGetTypeKind(type)==LLVMPointerTypeKind) {
    if (LLVMConstIntGetSExtValue(array[0])==0)
      ret = ValidIndexHelper(LLVMGetElementType(type),array+1,LLVMGetNumOperands(GEP)-2);

  } else {
     ret = ValidIndexHelper(type,array,LLVMGetNumOperands(GEP)-1);
  }

  free(array);
  return ret;
}

int CanReplace(LLVMValueRef I)
{
  if (!LLVMIsAAllocaInst(I))
    return 0;
  
  LLVMTypeRef type = LLVMGetElementType(LLVMTypeOf(I));

  LLVMTypeKind kind = LLVMGetTypeKind(type);

  // Check for struct or array type
  if (kind != LLVMStructTypeKind && kind != LLVMArrayTypeKind)
    {
      // not struct or array
      return 0;
    }

  LLVMUseRef use;
  for(use=LLVMGetFirstUse(I); use!=NULL; use=LLVMGetNextUse(use))
    {
      LLVMValueRef user = LLVMGetUser(use);
      // Each user of the alloca must be a getelementptr
      if (!LLVMIsAGetElementPtrInst(user)) {
	return 0;
      }
      
      // All ops (after the first one) of the gep must be
      // constant indices
      for(unsigned i=1; i<LLVMGetNumOperands(user); i++) 
	{
	  LLVMValueRef op = LLVMGetOperand(user,i);
	  if( !LLVMIsAConstantInt(op) )
	    return 0;
	}

      if (!ValidIndex(user))
	return 0;

      LLVMTypeRef type = LLVMTypeOf(user);
      // this is a pointer type
      if ( !IsScalarType(LLVMGetElementType(type)) ) {
	return 0;
      }      

      // Now we need to make sure that this gep is only used
      // loads and stores
      LLVMUseRef uu;
      for(uu=LLVMGetFirstUse(user); uu!=NULL; uu=LLVMGetNextUse(uu))
	{
	  if (!IsLoadOrStore(user,LLVMGetUser(uu))) {
	    return 0;
	  }
	}      
    }

  // All conditions passed
  return 1;
}

void DoScalarReplace(LLVMValueRef I) 
{
  LLVMBuilderRef Builder = LLVMCreateBuilder();
  LLVMPositionBuilderBefore(Builder,I);

  GEPMap map = GEPMapCreate();

  LLVMUseRef use;
  for(use=LLVMGetFirstUse(I); use!=NULL; use=LLVMGetNextUse(use))
    {
      LLVMValueRef gep = LLVMGetUser(use);      
      LLVMTypeRef scalarType = LLVMGetElementType(LLVMTypeOf(gep));

      LLVMValueRef alloca = GEPMapFind(map,gep);
      if (alloca==NULL) {
	alloca = LLVMBuildAlloca(Builder,scalarType,"");
	GEPMapInsert(map,gep,alloca);
      }

      LLVMReplaceAllUsesWith(gep,alloca);
      LLVMInstructionEraseFromParent(gep);
    }

  GEPMapDestroy(map);
  LLVMDisposeBuilder(Builder);
}

void RunScalarReplacement(LLVMModuleRef Module) 
{
  // Loop over all the functions
  LLVMValueRef F=NULL;

  worklist_t worklist = worklist_create();
  
  for(F=LLVMGetFirstFunction(Module);
      F!=NULL;
      F=LLVMGetNextFunction(F))
    {
      // Is this function defined?
      if (LLVMCountBasicBlocks(F)) 
	{
	  LLVMBasicBlockRef BB;
	  for(BB=LLVMGetFirstBasicBlock(F);
	      BB!=NULL;
	      BB=LLVMGetNextBasicBlock(BB))
	    {
	      LLVMValueRef I;
	      for (I=LLVMGetFirstInstruction(BB);
		   I!=NULL;
		   I=LLVMGetNextInstruction(I)) 
		{
		  if (CanReplace(I))
		    worklist_insert(worklist,I);
		}
	    }
	}
    }

  while(!worklist_empty(worklist)) {
    LLVMValueRef I = worklist_pop(worklist);
    DoScalarReplace(I);
  }

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
  
  RunScalarReplacement(Module);
  
  LLVMWriteBitcodeToFile(Module,argv[2]);

  return 0;
}
