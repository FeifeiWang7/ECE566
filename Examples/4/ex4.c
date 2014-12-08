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

int
main (int argc, char ** argv)
{
  LLVMModuleRef Module;

  /* Build global Module, this will be what we output */
  Module = LLVMModuleCreateWithName("main");

  // Make a function that's the instructions
  LLVMTypeRef VoidTy = LLVMVoidType();
  LLVMTypeRef VoidFnTy = LLVMFunctionType(VoidTy, NULL, 0, 0);

  LLVMValueRef Fn = LLVMAddFunction(Module,"main",VoidFnTy);
  //Now, we have a function inside our module

  //And, a basic block:
  LLVMBasicBlockRef entry = LLVMAppendBasicBlock(Fn,"entry");

  // Create Builder object that make instructions for us
  LLVMBuilderRef Builder = LLVMCreateBuilder();
  LLVMPositionBuilderAtEnd(Builder,entry);

  
  /*
    We'll make a for loop, something like this:

   entry:
      br label %for.cond

   for.cond:                                         ; preds = %for.body, %entry
      %i = phi i64 [ 0, %entry ], [ %iplus1, %for.body ]
      %0 = icmp ne i64 %i, 0
      br i1 %0, label %for.body, label %for.exit

   for.body:                                         ; preds = %for.cond
      %iplus1 = add i64 %i, 1
      br label %for.cond

   for.exit:                                         ; preds = %for.cond
      ret void
   */

  LLVMBasicBlockRef forcond = LLVMAppendBasicBlock(Fn,"for.cond");
  LLVMBasicBlockRef forbody = LLVMAppendBasicBlock(Fn,"for.body");
  LLVMBasicBlockRef forexit = LLVMAppendBasicBlock(Fn,"for.exit");

  LLVMBuildBr(Builder,forcond);

  /* Make the cond block:
   for.cond:                                         ; preds = %for.body, %entry
      %i = phi i64 [ 0, %entry ], [ %iplus1, %for.body ]
      %0 = icmp ne i64 %i, 0
      br i1 %0, label %for.body, label %for.exit

  */
  LLVMPositionBuilderAtEnd(Builder,forcond);
  
  LLVMValueRef phi_i = LLVMBuildPhi(Builder,LLVMInt64Type(),"i");
  LLVMBuildCondBr(Builder, 
		  LLVMBuildICmp(Builder,LLVMIntNE,phi_i,LLVMConstInt(LLVMInt64Type(),0,0),""),
		  forbody,forexit);
  
  /* Make the for.body:

   for.body:                                         ; preds = %for.cond
      %iplus1 = add i64 %i, 1
      br label %for.cond

   */
  LLVMPositionBuilderAtEnd(Builder,forbody);
  LLVMValueRef add = LLVMBuildAdd(Builder,phi_i,LLVMConstInt(LLVMInt64Type(),1,0),"iplus1");
  LLVMBuildBr(Builder,forcond);

  /* Make arguments for phi */
  LLVMValueRef incomingVals[2] = { LLVMConstInt(LLVMInt64Type(),0,0), add };
  LLVMBasicBlockRef incomingBBs[2] = { entry, forbody };
  
  LLVMAddIncoming(phi_i, incomingVals, incomingBBs, 2);
  
  /* Make the for.exit:

   for.exit:                                         ; preds = %for.cond
      ret void

   */
  LLVMPositionBuilderAtEnd(Builder,forexit);
  LLVMBuildRetVoid(Builder);

  LLVMWriteBitcodeToFile(Module,"main.bc");


  /*  LLVMUseRef use;
  fprintf(stderr,"All users of the instruction: "); LLVMDumpValue(phi_i);
  fprintf(stderr,"\n");
  for(use = LLVMGetFirstUse(phi_i); use!=NULL; use = LLVMGetNextUse(use)) 
    {
      LLVMValueRef user = LLVMGetUser(use);
      LLVMDumpValue(user);            
    }


  for(unsigned op=0; op< LLVMGetNumOperands(add); op++) {
    LLVMValueRef definition = LLVMGetOperand(add,op);
    fprintf(stderr,"Definition of op=%d is:",op); 
    LLVMDumpValue(definition);
  } 
  */

  /* Return an error status if it failed */
  return 0;
}
