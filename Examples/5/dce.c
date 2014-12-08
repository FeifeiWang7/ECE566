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

int isDead(LLVMValueRef I)
{
  LLVMOpcode opcode = LLVMGetInstructionOpcode(I);

  // Are there uses, if so not dead!
  if (LLVMGetFirstUse(I)!=NULL)
    return 0;

  switch(opcode) {
  case LLVMRet:
  case LLVMBr:
  case LLVMSwitch:
  case LLVMIndirectBr:
  case LLVMInvoke: 	
  case LLVMUnreachable:
  case LLVMFence:
  case LLVMStore:
  case LLVMCall:
  case LLVMAtomicCmpXchg:
  case LLVMAtomicRMW:
  case LLVMResume:	
  case LLVMLandingPad: 
    return 0;

  case LLVMLoad: if(LLVMGetVolatile(I)) return 0;
  default:
    break;
  }

  // All conditions passed
  return 1;
}

int DCE_count=0;

void RunDeadCodeElimination(LLVMModuleRef Module) 
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
		  if (isDead(I))
		    worklist_insert(worklist,I);
		}
	    }

	  while(!worklist_empty(worklist)) {
	    LLVMValueRef I = worklist_pop(worklist);
	     if (isDead(I))
	      {
		for(unsigned i=0; i<LLVMGetNumOperands(I); i++)
		  {
		    LLVMValueRef J = LLVMGetOperand(I,i);
		    if (LLVMIsAInstruction(J))
		      worklist_insert(worklist,J);
		  }
		DCE_count++;
		LLVMInstructionEraseFromParent(I);
	      }
	  }	  
	}
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
  
  RunDeadCodeElimination(Module);
  
  printf("DCE: %d eliminated\n",DCE_count);

  LLVMWriteBitcodeToFile(Module,argv[2]);

  return 0;
}
